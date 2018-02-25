#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <stdint.h>
#include "CardBase.hpp"


struct Card
{
	uint8_t id;
	CARD_TYPE type;
};

class Configuration
{
public:
	Configuration() = default;
	virtual ~Configuration() = default;
	static uint8_t GetCardAmount()
	{
		return cardAmount;
	}

	static Card* GetCardList()
	{
		return cardList;
	}
	static uint8_t GetCpuId()
	{
		return cpuId;
	}

private:
	static const uint8_t cpuId = 1;
	static const uint8_t cardAmount = 12;
	static Card cardList[cardAmount];
};

#endif // CONFIGURATION_HPP
