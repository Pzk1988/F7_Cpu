#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#include "stm32f7xx_it.h"
#include "Garbage.hpp"
#include <stdint.h>

extern CAN_HandleTypeDef    hcan1;

// Can Fifo
extern volatile struct CanRecData canRecData[10];
extern volatile uint8_t wCanRecData;
extern volatile uint8_t rCanRecData;

void SysTick_Handler(void)
{
	  HAL_IncTick();
	  HAL_SYSTICK_IRQHandler();
}

void CAN1_RX0_IRQHandler(void)
{
	if (HAL_CAN_GetRxMessage(&hcan1,
							 CAN_RX_FIFO0,
							 &(canRecData[wCanRecData].RxHeader),
							 (uint8_t*)(canRecData[wCanRecData].RxData)) != HAL_OK)
	{
	}
	if(wCanRecData == 9)
	{
		wCanRecData = 0;
	}
	else
	{
		wCanRecData++;
	}
}

