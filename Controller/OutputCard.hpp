#ifndef OUTPUTCARD_H
#define OUTPUTCARD_H

// Clib includes
// None

// Middleware include
// None

// Source include
#include "CardBase.hpp"

namespace Controller
{

class OutputCard: public CardBase
{
public:
	OutputCard(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId);
	virtual ~OutputCard() = default;
	void Init();
	void Process();
	void RxDataMsg(uint8_t *pData, uint8_t len) override;
	uint16_t GetState();

protected:
	// Serialiaze from unpacked to packed version
	void SerializeState();

	// Deserialize from packed to unpacked version
	void DeserializeState();

private:
	// Packed state, received from or ready to be send to card
	uint16_t packedState;

	// if remote request for data comes use this flag to send back data
	bool remoteRequest;
};

} // namespace Controller

#endif // OUTPUTCARD_H
