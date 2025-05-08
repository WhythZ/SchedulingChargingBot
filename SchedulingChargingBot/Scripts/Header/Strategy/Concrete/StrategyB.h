#ifndef _STRATEGY_B_H_
#define _STRATEGY_B_H_

#include "../Strategy.h"

struct StrategyB :public Strategy
{
	void UpdateMovement(Chargeable*) override;
};

#endif