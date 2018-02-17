#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include <string.h>
#include "Can.hpp"
#include "ICan.hpp"
#include "Garbage.hpp"
#include "lwip.h"

#include "Chassi.hpp"

CAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[8];
volatile bool sem = 0;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_LWIP_Init();

	// Can driver
	Driver::ICan* can = new Driver::Can(0x01);
	can->Init();

	// Chassi
	Controller::Chassi controller(can);

	while (1)
	{
		// Process tcp/ip stack
		MX_LWIP_Process();

		// Process chassi
		controller.Process();

		// Process can transmission
		if(sem == 1)
		{
			controller.RxMsg(RxHeader.StdId >> 5, RxData, RxHeader.DLC);
			sem = 0;
		}
	}
}

