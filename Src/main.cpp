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

// Forward declarations
static void Statistics();

// Global variable
Driver::ISerial* serial;

// Can Fifo
volatile struct CanRecData canRecData[10];
volatile uint8_t wCanRecData;
volatile uint8_t rCanRecData;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_LWIP_Init();

	// Can rec FIFO
	wCanRecData = 0;
	rCanRecData = 0;

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
		MX_LWIP_Process();

		// Process chassi
		controller->Process();

		// Process can fifo transmission
		// TODO: Create fifo class, think of race condition
		if(rCanRecData != wCanRecData)
		{

			controller->RxMsg((CAN_RxHeaderTypeDef*)&(canRecData[rCanRecData].RxHeader),
							  (uint8_t*)(canRecData[rCanRecData].RxData));
			if(rCanRecData == 9)
			{
				rCanRecData = 0;
			}
			else
			{
				rCanRecData++;
			}
		}

		// Calculate statistics
		Statistics();
	}
}

void Statistics()
{
	static uint32_t rounds = 0;
	static uint32_t prevTime = HAL_GetTick();

	rounds++;
	if(HAL_GetTick() - prevTime > 1000)
	{
		Logger::GetInstance()->Log("%d rounds/sec", rounds);
		rounds = 0;
		prevTime = HAL_GetTick();
	}
}

