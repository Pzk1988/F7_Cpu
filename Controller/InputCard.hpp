#ifndef INPUTCARD_H
#define INPUTCARD_H

// Clib includes
// None

// Middleware include
// None

// Source include
#include "CardBase.hpp"
#include "ICommunication.hpp"

namespace Controller
{

class InputCard: public CardBase
{
public:
	InputCard(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId);
	virtual ~InputCard() = default;
	void Init() {}
	void Process() {}
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
};

} // namespace Controller

#endif // INPUTCARD_H
