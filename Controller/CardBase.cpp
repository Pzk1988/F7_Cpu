// Clib includes
#include <cstdio>

// Middleware include
// None

// Source include
#include "CardBase.hpp"

namespace Controller
{


CardBase::CardBase(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId) : commDriver(commDriver),
cardId(cardId),
cpuId(cpuId)
{
	memset(unpackedState, 0, sizeof(unpackedState));
	memset(prevUnpackedState, 0, sizeof(prevUnpackedState));
}

uint8_t CardBase::GetId() const
{
	return cardId;
}

uint16_t* CardBase::GetSerializedDataAddress()
{
	return unpackedState;
}

}; // namespace Controller
