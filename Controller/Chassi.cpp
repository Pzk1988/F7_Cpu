#include "Chassi.hpp"
//#include "Garbage.hpp"
#include <stdio.h>
#include <algorithm>
#include "Logger.hpp"
#include "Configuration.hpp"
#include "InputCard.hpp"
#include "OutputCard.hpp"

namespace Controller
{

Chassi::Chassi(Driver::ICan* can) : can(can)
{
	for(size_t i = 0; i < Configuration::GetCardAmount(); i++)
	{
		switch(Configuration::GetCardList()[i].type)
		{
			case CARD_TYPE::INPUT:
			{
				cardsVector.push_back(new InputCard(can, Configuration::GetCardList()[i].id, Configuration::GetCpuId()));
				break;
			}
			case CARD_TYPE::OUTPUT:
			{
				cardsVector.push_back(new OutputCard(can, Configuration::GetCardList()[i].id, Configuration::GetCpuId()));
				break;
			}
			default:
				break;
		}
	}
}

void Chassi::ReadMsg(uint8_t senderId, uint8_t *pData, uint8_t len)
{
	auto it = std::find_if(cardsVector.begin(), cardsVector.end(), [senderId](ICard* card)
	{
		if(senderId == card->GetId())
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if(it != cardsVector.end())
	{
		(*it)->SetNewData(pData, len);
	}
	else
	{
		Logger::GetInstance()->Log("Id %d was not found");
	}
}

void Chassi::Process()
{
	std::for_each(cardsVector.begin(), cardsVector.end(), [](ICard* card)
	{
		card->Process();
	});
//	  if(HAL_GetTick() - prevTick > 2000)
//	  {
//		  prevTick = HAL_GetTick();
//
//		  // Frame to output card
//		  result |= can->DataFrame(0x02, (uint8_t*)&tmp, 2);
//		  if(result == HAL_OK)
//		  {
//			  char tab[40];
//			  sprintf(tab, "Frame to output %x\n", tmp);
//			  UdpWrite(tab, strlen(tab));
//		  }
//
//		  // Frame to input card
//		  result |= can->RemoteFrame(0x03);
//		  if(result == HAL_OK)
//		  {
//			  UdpWrite("Frame to input\n", 16);
//		  }
//
//		  // If error
//		  if( result != HAL_OK )
//		  {
//			  UdpWrite("Can spierdolina", 15);
//			  result = HAL_OK;
//		  }
//
//		  tmp++;
//	  }
}
} // namespace Controller
