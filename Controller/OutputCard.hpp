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
	void SetNewData(uint8_t *pData, uint8_t len);

private:
	uint16_t state;
};

} // namespace Controller

#endif // OUTPUTCARD_H
