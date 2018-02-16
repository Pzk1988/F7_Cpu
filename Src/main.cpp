#include "stm32f7xx_hal.h"
#include <string.h>
#include "Can.hpp"
#include "ICan.hpp"
#include "Garbage.hpp"
#include "lwip.h"

CAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[8];

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_LWIP_Init();

  UdpWrite( "Move to while", 13);
  uint32_t prevTick = HAL_GetTick();

  Driver::ICan* can = new Driver::Can();
  can->Init();

  uint8_t tab[8];
  memset(tab, 0x00, 8);
  uint8_t result = 0;

  while (1)
  {
	  MX_LWIP_Process();

	  if(HAL_GetTick() - prevTick > 2000)
	  {
		  prevTick = HAL_GetTick();
		  result |= can->RemoteFrame(0x01);
		  result |= can->DataFrame(0x02, tab, 8);

		  if( result != HAL_OK )
		  {
			  UdpWrite("Can spierdolina", 15);
			  result = HAL_OK;
		  }

		  tab[0]++;
		  memset(&tab[1], tab[0], 7);
	  }
  }
}

