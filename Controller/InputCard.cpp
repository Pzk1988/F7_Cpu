#include <InputCard.hpp>
#include <stm32f7xx_hal.h>
#include "Configuration.hpp"
#include "Garbage.hpp"

namespace Controller
{

InputCard::InputCard(Driver::ICan* can, uint8_t id, uint8_t cpuId) : ICard(can, id, cpuId)
{
}

void InputCard::Process()
{
	if(HAL_GetTick() - timeOfLastUpdate > 2000)
	{
		timeOfLastUpdate = HAL_GetTick();
		can->RemoteFrame(id);
	}
}

uint8_t InputCard::GetId() const
{
	return id;
}

void InputCard::SetNewData(uint8_t *pData, uint8_t len)
{
	if(len == 2)
	{
		timeOfLastUpdate = HAL_GetTick();
		state = pData[0] | (pData[1] << 8);

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
}

} // namespace Controller
