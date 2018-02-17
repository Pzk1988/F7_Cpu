#include <OutputCard.hpp>
#include "stm32f7xx_hal.h"
#include "Garbage.hpp"

namespace Controller
{

OutputCard::OutputCard(Driver::ICan* can, uint8_t id, uint8_t cpuId) : ICard(can, id, cpuId)
{
	state = 0;
}

void OutputCard::Process()
{
	if(HAL_GetTick() - timeOfLastUpdate > 2000)
	{
		timeOfLastUpdate = HAL_GetTick();
		can->DataFrame(id, (uint8_t*)&state, 2);
		state++;
	}
}

uint8_t OutputCard::GetId() const
{
	return id;
}

void OutputCard::SetNewData(uint8_t *pData, uint8_t len)
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
	tab[size] = '\n';
	UdpWrite(tab, size + 1);
}


} // namespace Controller
