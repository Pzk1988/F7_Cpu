#ifndef CHASSI_H
#define CHASSI_H
#include <stdint.h>
#include "ICan.hpp"
#include <vector>
#include "ICard.hpp"
#include "stm32f7xx_hal.h"

namespace Controller
{

class Chassi
{
public:
	Chassi(Driver::ICan* can);
	virtual ~Chassi() = default;
	void Init();
	void RxMsg(CAN_RxHeaderTypeDef* header, uint8_t *pData);
	void Process();

private:
	Driver::ICan* can;
	std::vector<ICard*> cardsVector;

	uint8_t** pInputs;
	uint8_t** pOutputs;
};

} // namespace Controller

#endif // CHASSI_H
