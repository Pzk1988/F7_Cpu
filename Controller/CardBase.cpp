// Clib includes
#include <cstdio>

// Middleware include
// None

// Source include
#include "CardBase.hpp"

namespace Controller
{


CardBase::CardBase(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId, CARD_TYPE type) : commDriver(commDriver),
cardId(cardId),
cpuId(cpuId),
type(type)
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

void CardBase::RxDataMsg(uint8_t *pData, uint8_t len)
{
	printf("RxDataMsg(uint8_t *pData, uint8_t len) not allowed\r\n");
	while(1){}
}

void CardBase::RxDataMsg(uint8_t *pData, uint32_t extId, uint8_t len)
{
	printf("RxDataMsg(uint8_t *pData, uint32_t extId, uint8_t len) not allowed\r\n");
	while(1){}
}

CARD_TYPE CardBase::GetType() const
{
	return type;
}

}; // namespace Controller
