#ifndef CHASSI_H
#define CHASSI_H
#include <stdint.h>
#include "ICan.hpp"
#include <vector>
#include "ICard.hpp"

namespace Controller
{

class Chassi
{
public:
	Chassi(Driver::ICan* can);
	virtual ~Chassi() = default;
	void RxMsg(uint8_t senderId, uint8_t *pData, uint8_t len);
	void Process();

private:
	Driver::ICan* can;
	std::vector<ICard*> cardsVector;
};

} // namespace Controller

#endif // CHASSI_H
