#ifndef _STRATEGY_C_H_
#define _STRATEGY_C_H_

#include "../Strategy.h"

class StrategyC :public Strategy
{
public:
	void UpdateMovement(Chargeable*) override;
};

#endif