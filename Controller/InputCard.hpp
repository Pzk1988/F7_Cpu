#ifndef INPUTCARD_H
#define INPUTCARD_H

#include <ICard.hpp>

namespace Controller
{

class InputCard: public ICard
{
public:
	InputCard(Driver::ICan* can, uint8_t id, uint8_t cpuId);
	virtual ~InputCard() = default;
	void Process();
	uint8_t GetId() const;
	void RxMsg(uint8_t *pData, uint8_t len);
	uint16_t GetState();

private:
	uint16_t state;
};

} // namespace Controller

#endif // INPUTCARD_H
