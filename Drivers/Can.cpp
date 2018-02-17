#include <Can.hpp>
#include "stm32f746xx.h"
#include "stm32f7xx_hal_can.h"

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
	hcan1.Init.AutoRetransmission = DISABLE;
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

bool Can::DataFrame(uint16_t id, uint8_t* pData, uint8_t len)
{
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

bool Can::RemoteFrame(uint16_t id)
{
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


//  if(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) != 1)
//  {
//    Error_Handler();
//  }
//
//  if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  if((RxHeader.StdId != 0x11)                     ||
//     (RxHeader.RTR != CAN_RTR_DATA)               ||
//     (RxHeader.IDE != CAN_ID_STD)                 ||
//     (RxHeader.DLC != 2)                          ||
//     ((RxData[0]<<8 | RxData[1]) != 0x55aa))
//  {
//    return HAL_ERROR;
//  }
