#ifndef DRIVER_SERIAL_H
#define DRIVER_SERIAL_H

#include "ISerial.hpp"
#include "stm32f7xx_hal.h"

namespace Driver
{

class Serial : public ISerial
{
public:
	Serial() = default;
	virtual ~Serial() = default;
	void Init(int baudRate);
	int Write(const char* str);
	int Put(char c);

private:
	UART_HandleTypeDef huart3;
};

};
#endif // DRIVER_SERIAL_H


