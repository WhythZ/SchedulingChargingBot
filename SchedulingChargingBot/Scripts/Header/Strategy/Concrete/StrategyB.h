#ifndef _STRATEGY_B_H_
#define _STRATEGY_B_H_

#include "../Strategy.h"
#include "../../Chargeable/Concrete/Vehicle.h"

class StrategyB :public Strategy
{
public:
	void UpdateMovement(Chargeable*) override;

private:
	double CalculateVehicleValue(Vehicle*, double, double, double);
};

#endif