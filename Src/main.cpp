#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include <string.h>
#include "Can.hpp"
#include "ICan.hpp"
#include "Garbage.hpp"
#include "lwip.h"
#include <stdio.h>

CAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[8];
volatile bool sem = 0;

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

  uint16_t tmp = 0;
  uint8_t result = 0;

  while (1)
  {
	  MX_LWIP_Process();

	  if(HAL_GetTick() - prevTick > 2000)
	  {
		  prevTick = HAL_GetTick();

		  // Frame to output card
		  result |= can->DataFrame(0x02, (uint8_t*)&tmp, 2);
		  if(result == HAL_OK)
		  {
			  char tab[40];
			  sprintf(tab, "Frame to output %x", tmp);
			  UdpWrite(tab, strlen(tab));
		  }

		  // Frame to input card
		  result |= can->RemoteFrame(0x03);
		  if(result == HAL_OK)
		  {
			  UdpWrite("Frame to input", 15);
		  }

		  // If error
		  if( result != HAL_OK )
		  {
			  UdpWrite("Can spierdolina", 15);
			  result = HAL_OK;
		  }

		  tmp++;
	  }

	  if(sem == 1)
	  {
		  char tab[100];
		  int size = sprintf(tab, "Msg from %d, ", RxHeader.StdId);
		  for(size_t i = 0; i < RxHeader.DLC; i++)
		  {
			  int size1 = sprintf(&tab[size], "%x, ", RxData[i]);
			  size += size1;

		  }
		  UdpWrite(tab, size);
		  sem = 0;
	  }
  }
}

