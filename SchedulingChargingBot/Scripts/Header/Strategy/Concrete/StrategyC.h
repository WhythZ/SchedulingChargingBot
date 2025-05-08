#ifndef _STRATEGY_C_H_
#define _STRATEGY_C_H_

#include "../Strategy.h"

struct StrategyC :public Strategy
{
	void UpdateMovement(Chargeable*) override;
};

#endif