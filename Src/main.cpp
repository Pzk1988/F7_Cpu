// Clib includes
#include <string.h>

// Middleware include
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include "lwip.h"

// Source include
#include "Can.hpp"
#include "ICan.hpp"
#include "Garbage.hpp"
#include "Logger.hpp"
#include "Chassi.hpp"
#include "Serial.hpp"

// Global variable
Driver::ISerial* serial;
CAN_RxHeaderTypeDef   RxHeader;
volatile uint8_t RxData[8];
volatile bool sem = 0;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	//MX_LWIP_Init();

	// Serial
	serial = new Driver::Serial();
	serial->Init(115200);

	Logger::GetInstance()->Log("Tcp/ip stack initialized");

	// Can driver
	Driver::ICan* can;
	can = new Driver::Can(0x01);
	can->Init();

	// Chassi
	Controller::Chassi* controller;
	controller = new Controller::Chassi(can);
	controller->Init();


	while (1)
	{
		// Process tcp/ip stack
		//MX_LWIP_Process();

		// Process chassi
		controller->Process();

		// Process can transmission
		if(sem == 1)
		{
			controller->RxMsg(RxHeader.StdId >> 5, (uint8_t*)(RxData), RxHeader.DLC);
			sem = 0;
		}
	}
}

