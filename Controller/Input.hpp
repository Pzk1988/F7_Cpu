#ifndef IINPUT_HPP
#define IINPUT_HPP
#include <stdint.h>

namespace Controller
{

enum class INPUT_TYPE
{
	DIGITA,
	ANALOG,
	TEMPERATURE
};

class Input
{
public:
	Input(uint8_t index, INPUT_TYPE type);
	virtual ~Input() = default;
	void SetState(uint8_t state);
	uint8_t GetState();

private:
	uint8_t index;
	INPUT_TYPE type;
	uint8_t state;
	bool outOfControl;
};

}; // namespace Controller

#endif // IINPUT_HPP

