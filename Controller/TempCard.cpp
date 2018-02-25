#include <TempCard.hpp>
#include "Logger.hpp"

namespace Controller
{

TempCard::TempCard(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId) :
		  CardBase(commDriver, cardId, cpuId, CARD_TYPE::TEMPERATURE)
{
	commDriver->SendRemoteFrame(cardId);
}

void TempCard::RxDataMsg(uint8_t *pData, uint32_t extId, uint8_t len)
{
	Logger::GetInstance()->Log(pData, extId, len, cardId);
	memcpy(&unpackedState[4 * (extId)], pData, len);
}

uint16_t TempCard::GetState(uint8_t sensor)
{
	return 0;
}

void TempCard::DeserializeState()
{

}

void TempCard::SerializeState()
{

}

} /* namespace Controller */
