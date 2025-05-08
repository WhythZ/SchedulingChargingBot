#include "../../../Header/Strategy/Concrete/StrategyA.h"

void StrategyA::UpdateMovement(Chargeable* _robot)
{
	_robot->SetVelocity({ 50,50 });
}