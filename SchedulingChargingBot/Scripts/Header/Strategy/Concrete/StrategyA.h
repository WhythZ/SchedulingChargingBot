#ifndef _STRATEGY_A_HPP_
#define _STRATEGY_A_HPP_

#include "../Strategy.h"

struct StrategyA :public Strategy
{
	void UpdateMovement(Chargeable*) override;
};

#endif