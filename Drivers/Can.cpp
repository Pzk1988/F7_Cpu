#include <Can.hpp>
#include "stm32f746xx.h"
#include "stm32f7xx_hal_can.h"
#include "Logger.hpp"

CAN_HandleTypeDef 	hcan1;

namespace Driver
{

Can::Can(uint8_t ownId) : ownId(ownId)
{

}

uint8_t Can::Init()
{
	uint8_t ret = HAL_OK;

	hcan1.Instance = CAN1;
	HAL_CAN_DeInit(&hcan1);
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
	hcan1.Init.Prescaler = 6;

	if(HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		return HAL_ERROR;
	}

	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	{
		return HAL_ERROR;
	}

	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		return HAL_ERROR;
	}

	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		return HAL_ERROR;
	}

	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

	return ret;
}

uint8_t Can::Init(uint8_t filterId)
{
	uint8_t ret = HAL_OK;
	HAL_CAN_DeInit(&hcan1);

	hcan1.Instance = CAN1;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
	hcan1.Init.Prescaler = 6;

	if(HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		return HAL_ERROR;
	}

	CAN_FilterTypeDef  sFilterConfig;

	uint32_t filter_mask = 0x00000000;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = ((filterId << 5)  | (filterId >> (32 - 5))) & 0xFFFF; // STID[10:0] & EXTID[17:13]
	sFilterConfig.FilterIdLow = (filterId >> (11 - 3)) & 0xFFF8; // EXID[12:5] & 3 Reserved bits
	sFilterConfig.FilterMaskIdHigh = ((filter_mask << 5)  | (filter_mask >> (32 - 5))) & 0xFFFF;
	sFilterConfig.FilterMaskIdLow = (filter_mask >> (11 - 3)) & 0xFFF8;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	//sFilterConfig.SlaveStartFilterBank = 14;

	if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	{
		return HAL_ERROR;
	}

	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		return HAL_ERROR;
	}

	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		return HAL_ERROR;
	}

	return ret;
}

bool Can::SendDataFrame(uint16_t id, uint8_t* pData, uint8_t len)
{
	CAN_TxHeaderTypeDef	TxHeader;
	uint32_t TxMailbox;

	char tab[100];
	int size = sprintf(tab, "Data to %d ", id);
	for(size_t i = 0; i < len; i++)
	{
		size += sprintf(&tab[size], "0x%x ", pData[i]);
	}
	Logger::GetInstance()->Log(tab);

	TxHeader.StdId = (ownId << 5) | id;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = len;
	TxHeader.TransmitGlobalTime = DISABLE;

	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, pData, &TxMailbox) != HAL_OK)
	{
		return HAL_ERROR;
	}

	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3) {}

	return HAL_OK;
}

bool Can::SendDataFrame(uint16_t id, uint32_t extId, uint8_t* pData, uint8_t len)
{
	CAN_TxHeaderTypeDef	TxHeader;
	uint32_t TxMailbox;

	char tab[100];
	int size = sprintf(tab, "Data to %d ", id);
	for(size_t i = 0; i < len; i++)
	{
		size += sprintf(&tab[size], "0x%x ", pData[i]);
	}
	Logger::GetInstance()->Log(tab);

	TxHeader.StdId = (ownId << 5) | id;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.ExtId = extId;
	TxHeader.DLC = len;
	TxHeader.TransmitGlobalTime = DISABLE;

	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, pData, &TxMailbox) != HAL_OK)
	{
		return HAL_ERROR;
	}

	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3) {}

	return HAL_OK;
}

bool Can::SendRemoteFrame(uint16_t id)
{
	CAN_TxHeaderTypeDef	TxHeader;
	uint32_t TxMailbox;

	Logger::GetInstance()->Log("Remote to 0x%x", id);
	TxHeader.StdId = (ownId << 5) | id;
	TxHeader.RTR = CAN_RTR_REMOTE;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 0;
	TxHeader.TransmitGlobalTime = DISABLE;
	uint8_t tab[8];

	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, tab, &TxMailbox) != HAL_OK)
	{
		return HAL_ERROR;
	}

	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3) {}

	return HAL_OK;
}

} // namespace Driver


