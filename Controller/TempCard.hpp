#ifndef TEMPCARD_H
#define TEMPCARD_H
// Clib includes
// None

// Middleware include
// None

// Source include
#include "CardBase.hpp"

namespace Controller
{

class TempCard: public CardBase
{
public:
	TempCard(Driver::ICommunication* commDriver, uint8_t cardId, uint8_t cpuId);
	virtual ~TempCard() = default;
	void Init() {}
	void Process() {}
	void RxDataMsg(uint8_t *pData, uint32_t extId, uint8_t len) override;
	uint16_t GetState(uint8_t sensor);

protected:
	// Serialiaze from unpacked to packed version
	void SerializeState();

	// Deserialize from packed to unpacked version
	void DeserializeState();

private:
	static const uint16_t PACKED_DATA_SIZE = 20;
	static const uint16_t BITS_PER_ONE_SENSOR = 10;

	// Packed state, received from or ready to be send to card
	uint16_t packedState[PACKED_DATA_SIZE];
};

} // namespace Controller

#endif // TEMPCARD_H
