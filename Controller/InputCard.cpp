#include <InputCard.hpp>
#include <stm32f7xx_hal.h>
#include "Configuration.hpp"
#include "Logger.hpp"

namespace Controller
{

InputCard::InputCard(Driver::ICan* can, uint8_t id, uint8_t cpuId) : ICard(can, id, cpuId)
{
	state = 0;
	can->RemoteFrame(id);
}

void InputCard::Init()
{

}

void InputCard::Process()
{

}

uint8_t InputCard::GetId() const
{
	return id;
}

void InputCard::RxMsg(uint8_t *pData, uint8_t len)
{
	if(len == 2)
	{
		timeOfLastUpdate = HAL_GetTick();
		state = pData[0] | (pData[1] << 8);
		SerializeState();

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
	}
}

uint16_t InputCard::GetState()
{
	return state;
}

void InputCard::SerializeState()
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
