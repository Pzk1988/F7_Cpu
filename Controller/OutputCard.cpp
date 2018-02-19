#include <OutputCard.hpp>
#include "stm32f7xx_hal.h"
#include "Logger.hpp"

namespace Controller
{

OutputCard::OutputCard(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId) :
		      CardBase(commDriver, cardId, cpuId)
{
	packedState = 0;
	remoteRequest = 0;
}

void OutputCard::Init()
{
	uint16_t initData = 0;

	commDriver->SendDataFrame(cardId, (uint8_t*)&initData, 2);
}

void OutputCard::Process()
{
	if(memcmp(prevUnpackedState, unpackedState, sizeof(prevUnpackedState)) != 0 ||
	   remoteRequest == true)
	{
		remoteRequest = false;
		memcpy(prevUnpackedState, unpackedState, sizeof(prevUnpackedState));
		SerializeState();
		commDriver->SendDataFrame(cardId, (uint8_t*)&packedState, 2);
	}
}

void OutputCard::RxDataMsg(uint8_t *pData, uint8_t len)
{
	if(len == 0) // TODO: Think if there should be separate method for remote msg
	{
		remoteRequest = true;
	}
	Logger::GetInstance()->Log(pData, len, cardId);
}

uint16_t OutputCard::GetState()
{
	return packedState;
}

void OutputCard::DeserializeState()
{
	for(size_t i = 0; i < DEVICE_AMOUNT; i++)
	{
		if((packedState & (1 << i)) != 0)
		{
			unpackedState[i] = 1;
		}
		else
		{
			unpackedState[i] = 0;
		}
	}
}

void OutputCard::SerializeState()
{
	packedState = 0;
	for(size_t i = 0; i < DEVICE_AMOUNT; i++)
	{
		if(unpackedState[i] == 1)
		{
			packedState |= (1 << i);
		}
	}
}


} // namespace Controller
