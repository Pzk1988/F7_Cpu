// Clib includes
// None

// Middleware include
#include <stm32f7xx_hal.h>

// Source include
#include "InputCard.hpp"
#include "Configuration.hpp"
#include "Logger.hpp"

namespace Controller
{

InputCard::InputCard(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId) :
		   CardBase(commDriver, cardId, cpuId, CARD_TYPE::INPUT)
{
	packedState = 0;
	commDriver->SendRemoteFrame(cardId);
}

void InputCard::RxDataMsg(uint8_t *pData, uint8_t len)
{
	packedState = pData[0] | (pData[1] << 8);
	DeserializeState();

	Logger::GetInstance()->Log(pData, len, cardId);
}

uint16_t InputCard::GetState()
{
	return packedState;
}

void InputCard::DeserializeState()
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

void InputCard::SerializeState()
{
	packedState = 0;

	for( size_t i = 0; i < DEVICE_AMOUNT; i++ )
	{
		if( unpackedState[i] == 1 )
		{
			packedState |= (1 << i);
		}
	}
}
} // namespace Controller
