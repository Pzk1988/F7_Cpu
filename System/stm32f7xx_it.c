#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#include "stm32f7xx_it.h"
#include "Garbage.hpp"
#include <stdint.h>

extern CAN_HandleTypeDef    hcan1;
extern CAN_RxHeaderTypeDef   RxHeader;
extern uint8_t               RxData[8];
extern volatile uint8_t sem;

void SysTick_Handler(void)
{
	  HAL_IncTick();
	  HAL_SYSTICK_IRQHandler();
}

void CAN1_RX0_IRQHandler(void)
{
	  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	  {
	  }
	  sem = 1;
}

