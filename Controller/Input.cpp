#include <Input.hpp>

namespace Controller
{

Input::Input(uint8_t index, INPUT_TYPE type) : index(index), type(type)
{

}

void Input::SetState(uint8_t state)
{
	this->state = state;
}

uint8_t Input::GetState()
{
	return state;
}

} /* namespace Controller */
