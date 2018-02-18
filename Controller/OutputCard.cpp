#include <OutputCard.hpp>
#include "stm32f7xx_hal.h"
#include "Logger.hpp"

namespace Controller
{

OutputCard::OutputCard(Driver::ICan* can, uint8_t id, uint8_t cpuId) : ICard(can, id, cpuId)
{
	state = 0;
	stateChanged = false;
}

void OutputCard::Init()
{
	uint8_t data[2];
	data[0] = 0;
	data[1] = 0;

	can->DataFrame(id, data, 2);
}

void OutputCard::Process()
{
	if(stateChanged == true)
	{
		stateChanged = false;
		timeOfLastUpdate = HAL_GetTick();
		can->DataFrame(id, (uint8_t*)&state, 2);
	}
}

uint8_t OutputCard::GetId() const
{
	return id;
}

void OutputCard::RxMsg(uint8_t *pData, uint8_t len)
{
	char tab[100];
	int size = sprintf(tab, "Msg from 0x%x", id);
	if(len != 0)
	{
		int size1 = sprintf(&tab[size], ": ");
		size += size1;
	}
	for(size_t i = 0; i < len; i++)
	{
		int size1 = sprintf(&tab[size], " 0x%x", pData[i]);
		size += size1;

	}
	Logger::GetInstance()->Log(tab);
	if(len == 0)
	{
		stateChanged = true;
	}
}

void OutputCard::SetState(uint16_t newState)
{
	if(state != newState)
	{
		state = newState;
		stateChanged = true;
		SerializeState();
	}
}

uint16_t OutputCard::GetState()
{
	return state;
}
#warning think of better way to send data after change, if someone set serializedState than card does not know about it yet
void OutputCard::SerializeState()
{
	for(size_t i = 0; i < DATA_SIZE; i++)
	{
		if((state & (1 << i)) != 0)
		{
			serializedState[i] = 1;
		}
		else
		{
			serializedState[i] = 0;
		}
	}
}


} // namespace Controller
