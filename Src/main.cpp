// Clib includes
#include <ICommunication.hpp>
#include <string.h>

// Middleware include
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include "lwip.h"
#include <lwip/tcp.h>
#include <lwip/err.h>

// Source include
#include "Can.hpp"
#include "Garbage.hpp"
#include "Logger.hpp"
#include "Chassi.hpp"
#include "Serial.hpp"
#include "ICommunication.hpp"

// Forward declarations
static void Statistics();
static void WebServer();
static void WebServerInit();
static err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb, err_t err);

// Global variable
Driver::ISerial* serial;
tcp_pcb* tcp_echoserver_pcb;

// Can Fifo
volatile struct CanRecData canRecData[10];
volatile uint8_t wCanRecData;
volatile uint8_t rCanRecData;

int main(void)
{
	HAL_Init();

	SCB_EnableICache();
	SCB_EnableDCache();

	SystemClock_Config();
	MX_GPIO_Init();
	MX_LWIP_Init();
	WebServerInit();

	// Can rec FIFO
	wCanRecData = 0;
	rCanRecData = 0;

	// Serial
	serial = new Driver::Serial();
	serial->Init(115200);

	Logger::GetInstance()->Log("Tcp/ip stack initialized");

	// Can driver
	Driver::ICommunication* canDriver;
	canDriver = new Driver::Can(0x01);
	canDriver->Init();

	// Chassi
	Controller::Chassi* controller;
	controller = new Controller::Chassi(canDriver);
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
//		Statistics();

		// Handle web server
		void WebServer();
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

void WebServerInit()
{
	tcp_echoserver_pcb = tcp_new();

	if (tcp_echoserver_pcb != NULL)
	{
		err_t err;

		/* bind echo_pcb to port 7 (ECHO protocol) */
		err = tcp_bind(tcp_echoserver_pcb, IP_ADDR_ANY, 80);

		if (err == ERR_OK)
		{
			/* start tcp listening for echo_pcb */
			tcp_echoserver_pcb = tcp_listen(tcp_echoserver_pcb);

			/* initialize LwIP tcp_accept callback function */
			tcp_accept(tcp_echoserver_pcb, tcp_echoserver_accept);
		}
		else
		{
			printf("Can not bind pcb\n");
		}
	}
	else
	{
		printf("Can not create new pcb\n");
	}
}

err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	tcp_write(newpcb, "Hello world", 11, 0);
	tcp_output(newpcb);
	printf("Callback\r\n");
	return ERR_OK;
}
