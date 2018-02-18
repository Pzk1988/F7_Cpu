#ifndef ICARD_HPP
#define ICARD_HPP
#include <stdint.h>
#include "ICan.hpp"

namespace Controller
{

class ICard
{
public:
	ICard(Driver::ICan* can, uint8_t id, uint8_t cpuId) : can(can), id(id), cpuId(cpuId)
	{
		timeOfLastUpdate = 0;
	}
	virtual void Init() = 0;
	virtual void Process() = 0;
	virtual uint8_t GetId() const = 0;
	virtual void RxMsg(uint8_t *pData, uint8_t len) = 0;

protected:
	Driver::ICan* can;
	uint8_t id;
	uint8_t cpuId;
	uint32_t timeOfLastUpdate;
};

}

#endif // ICARD_HPP
