#include "Chassi.hpp"
#include <stdio.h>
#include <algorithm>
#include "Logger.hpp"
#include "Configuration.hpp"
#include "InputCard.hpp"
#include "OutputCard.hpp"
#include "stm32f7xx_hal_can.h"
#include <string.h>

namespace Controller
{

Chassi::Chassi(Driver::ICan* can) : can(can), pInputs(nullptr), pOutputs(nullptr) {}

void Chassi::Init()
{
	ICard* card = nullptr;
	uint8_t inputsAmount = 0;
	uint8_t outputsAmount = 0;

	// Init cards
	for(size_t i = 0; i < Configuration::GetCardAmount(); i++)
	{
		switch(Configuration::GetCardList()[i].type)
		{
			case CARD_TYPE::INPUT:
			{
				card = new InputCard(can, Configuration::GetCardList()[i].id, Configuration::GetCpuId());
				card->Init();
				cardsVector.push_back(card);
				inputsAmount++;
				break;
			}
			case CARD_TYPE::OUTPUT:
			{
				card = new OutputCard(can, Configuration::GetCardList()[i].id, Configuration::GetCpuId());
				card->Init();
				cardsVector.push_back(card);
				outputsAmount++;
				break;
			}
			case CARD_TYPE::OUTPUT_PWM:
			{
				outputsAmount++;
				break;
			}
			case CARD_TYPE::OUTPUT_TRIAK:
			{
				outputsAmount++;
				break;
			}
			case CARD_TYPE::TEMPERATURE:
			{
				inputsAmount++;
				break;
			}
			default:
				int type = static_cast<std::underlying_type<CARD_TYPE>::type>(Configuration::GetCardList()[i].type);
				Logger::GetInstance()->Log("Card type %u not found", type);
				break;
		}
	}

	// Init inputs and outputs for logic expressions
	pInputs = new uint8_t*[inputsAmount];
	uint8_t inIndex = 0;
	pOutputs = new uint8_t*[outputsAmount];
	uint8_t outIndex = 0;

	for(size_t i = 0; i < Configuration::GetCardAmount(); i++)
	{
		switch(Configuration::GetCardList()[i].type)
		{
			case CARD_TYPE::INPUT:
			case CARD_TYPE::TEMPERATURE:
			{
				pInputs[inIndex++] = cardsVector[i]->GetSerializedDataAddress();
				break;
			}
			case CARD_TYPE::OUTPUT:
			case CARD_TYPE::OUTPUT_PWM:
			case CARD_TYPE::OUTPUT_TRIAK:
			{
				pOutputs[outIndex++] = cardsVector[i]->GetSerializedDataAddress();
				break;
			}

			default:
				int type = static_cast<std::underlying_type<CARD_TYPE>::type>(Configuration::GetCardList()[i].type);
				Logger::GetInstance()->Log("Logic expressions card type %u not found", type);
				break;
		}
	}
}

void Chassi::RxMsg(CAN_RxHeaderTypeDef* header, uint8_t *pData)
{
	uint16_t senderId = header->StdId >> 5;
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
		(*it)->RxMsg(pData, header->DLC);
	}
	else
	{
		Logger::GetInstance()->Log("Id %u was not found", senderId);
	}
}

void Chassi::Process()
{
	// Send output state, check input state
	std::for_each(cardsVector.begin(), cardsVector.end(), [](ICard* card)
	{
		card->Process();
	});

	// Calculate logic expressions
	memcpy(pOutputs[0], pInputs[0], 16);

	static uint32_t prevTime = HAL_GetTick();

	if(HAL_GetTick() - prevTime > 1000)
	{
		prevTime = HAL_GetTick();
		printf("Card 0: ");
		for(int i = 0; i < 16; i++)
		{
			printf("%d ", pInputs[0][i]);
		}

		printf("\r\nCard 1: ");
		for(int i = 0; i < 16; i++)
		{
			printf("%d ", pOutputs[0][i]);
		}
		printf("\r\n");
	}
}
} // namespace Controller

