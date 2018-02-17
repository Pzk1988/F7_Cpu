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

void Chassi::RxMsg(uint8_t senderId, uint8_t *pData, uint8_t len)
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
		(*it)->RxMsg(pData, len);
	}
	else
	{
		Logger::GetInstance()->LogV("Id %d was not found", senderId);
	}
}

void Chassi::Process()
{
	std::for_each(cardsVector.begin(), cardsVector.end(), [](ICard* card)
	{
		card->Process();
	});

	static uint16_t prevState = 0;
	uint16_t inState = ((InputCard*)cardsVector[1])->GetState();
	if(inState != prevState)
	{
		((OutputCard*)cardsVector[0])->SetState(inState);
	}
	prevState = inState;
}
} // namespace Controller
