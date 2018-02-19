#include "TempCard.h"

namespace Controller
{

TempCard::TempCard(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId) :
		  CardBase(commDriver, cardId, cpuId)
{

}

void TempCard::RxDataMsg(uint8_t sensorSet, uint8_t *pData, uint8_t len)
{

}

uint16_t TempCard::GetState(uint8_t sensor)
{

}

void TempCard::DeserializeState()
{
#error check correctness
	unpackedState[i] =      packedState[i]           | ((packedState[i + 1] & 0x03) << 8);
	unpackedState[i + 1] = (packedState[i + 1] >> 2) | ((packedState[i + 2] & 0x0F) << 4);
	unpackedState[i + 2] = (packedState[i + 2] >> 4) | ((packedState[i + 3] & 0x3F) << 6);
	unpackedState[i + 3] = (packedState[i + 3] >> 6) | ((packedState[i + 4] & 0xFF) << 8);


	for(size_t i = 0; i < DEVICE_AMOUNT; i++)
	{
		unpackedState[i] = packedState
	}
}

void TempCard::SerializeState()
{

}

} /* namespace Controller */
