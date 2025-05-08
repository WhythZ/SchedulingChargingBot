#include "../../../Header/Strategy/Concrete/StrategyC.h"

void StrategyC::UpdateMovement(Chargeable* _robot)
{
	_robot->SetVelocity({ -100,-100 });
}