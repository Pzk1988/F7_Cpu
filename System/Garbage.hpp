#ifndef GARBAGE_HPP
#define GARBAGE_HPP
#include <stm32f7xx_hal.h>
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void UdpWrite(const char *ptr, int len);
void _Error_Handler(const char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)


extern UART_HandleTypeDef huart3;

struct CanRecData
{
	CAN_RxHeaderTypeDef   RxHeader;
	volatile uint8_t RxData[8];
};

#endif
