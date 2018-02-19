#ifndef CARDBASE_HPP
#define CARDBASE_HPP
#include <ICommunication.hpp>
#include <stdint.h>
#include <cstring>

namespace Controller
{

class CardBase
{
public:
	CardBase(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId);
	virtual void Init() = 0;
	virtual void Process() = 0;
	uint8_t GetId() const;
	virtual void RxDataMsg(uint8_t *pData, uint8_t len) = 0;
	uint16_t* GetSerializedDataAddress();

protected:
	// Amount of device per card
	const static uint8_t DEVICE_AMOUNT = 16;

	// Can driver
	Driver::ICommunication* commDriver;

	// Card id
	uint8_t cardId;

	// Cpu card id
	uint8_t cpuId;

	// Time of last communication with card
	uint32_t lastCommunicationTime;

	// Unpacked state, ready for calculation of logic expressions
	uint16_t unpackedState[DEVICE_AMOUNT];
	uint16_t prevUnpackedState[DEVICE_AMOUNT];

	// Serialiaze from unpacked to packed version
	virtual void SerializeState() = 0;

	// Deserialize from packed to unpacked version
	virtual void DeserializeState() = 0;
};

}

#endif // CARDBASE_HPP
