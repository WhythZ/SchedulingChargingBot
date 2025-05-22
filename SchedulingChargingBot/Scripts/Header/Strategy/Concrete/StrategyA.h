#ifndef _STRATEGY_A_HPP_
#define _STRATEGY_A_HPP_

#include "../Strategy.h"

class StrategyA :public Strategy
{
public:
	void UpdateMovement(Chargeable*) override;
};

#endif