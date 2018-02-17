#ifndef OUTPUTCARD_H
#define OUTPUTCARD_H

#include <ICard.hpp>

namespace Controller
{

class OutputCard: public ICard
{
public:
	OutputCard(Driver::ICan* can, uint8_t id, uint8_t cpuId);
	virtual ~OutputCard() = default;
	void Process();
	uint8_t GetId() const;
	void RxMsg(uint8_t *pData, uint8_t len);
	void SetState(uint16_t newState);
	uint16_t GetState();

private:
	uint16_t state;
	bool stateChanged;
};

} // namespace Controller

#endif // OUTPUTCARD_H
