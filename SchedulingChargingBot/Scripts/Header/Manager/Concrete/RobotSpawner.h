#ifndef _ROBOT_SPAWNER_H_
#define _ROBOT_SPAWNER_H_

#include <vector>
#include <queue>
#include "../../Chargeable/Concrete/Robot.h"
#include "../../Strategy/Concrete/StrategyA.h"
#include "../../Strategy/Concrete/StrategyB.h"
#include "../../Strategy/Concrete/StrategyC.h"

class RobotSpawner
{
public:
    void CreateRobot(int level);  //���ز�ͬ��ģ

private:
    void BornPlaceCreate(double&, double&, int&, int&);
};

#endif
