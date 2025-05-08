#include "../../../Header/Strategy/Concrete/StrategyB.h"

void StrategyB::UpdateMovement(Chargeable* _robot)
{
	_robot->SetVelocity({ 100,100 });
}