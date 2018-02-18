// System includes
#include <string.h>
#include <Serial.hpp>

// Project includes

namespace Driver
{

void Serial::Init(int baudRate)
{
	/* Init the low level hardware : GPIO, CLOCK */
	__HAL_RCC_USART3_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart3);
}

int Serial::Write(const char* str)
{
	size_t len = strlen(str);
	for(size_t i = 0; i < len; i++)
	{
		Put(*str++);
	}

	return len;
}

int Serial::Put(char c)
{
	HAL_UART_Transmit(&huart3, (uint8_t*)&c, 1, 200);
	return 1;
}
};
