#ifndef CHASSI_H
#define CHASSI_H

// Clib includes
#include <stdint.h>
#include <vector>

// Middleware include
#include "stm32f7xx_hal.h"

// Source include
#include <CardBase.hpp>
#include <ICommunication.hpp>

namespace Controller
{

class Chassi
{
public:
	Chassi(Driver::ICommunication* commDriver);
	virtual ~Chassi() = default;
	void Init();
	void RxMsg(CAN_RxHeaderTypeDef* header, uint8_t *pData);
	void Process();

private:
	Driver::ICommunication* commDriver;
	std::vector<CardBase*> cardsVector;

	uint16_t** pInputs;
	uint16_t** pOutputs;
};

} // namespace Controller

#endif // CHASSI_H
