#ifndef CAN_HPP
#define CAN_HPP

#include <ICommunication.hpp>
#include "stm32f7xx_hal.h"
#include <stdint.h>


namespace Driver
{

class Can : public ICommunication
{
public:
	Can(uint8_t ownId);
	uint8_t Init();
	uint8_t Init(uint8_t filterId);
	bool SendDataFrame(uint16_t id, uint8_t* pData, uint8_t len);
	bool SendRemoteFrame(uint16_t id);
	virtual ~Can() = default;

private:
	CAN_TxHeaderTypeDef	TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailbox;
	uint8_t ownId;
};

} // namespace Driver

#endif // CAN_HPP
