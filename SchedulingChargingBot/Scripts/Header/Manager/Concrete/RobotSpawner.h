#ifndef _ROBOT_SPAWNER_H_
#define _ROBOT_SPAWNER_H_

#include <vector>
#include <queue>
#include "../../Chargeable/Concrete/Robot.h"
#include "../../Strategy/Concrete/StrategyA.h"
#include "../../Strategy/Concrete/StrategyB.h"
#include "../../Strategy/Concrete/StrategyC.h"
class RobotSpawner {
private:
    StrategyA strategyA;
    StrategyB strategyB;
    StrategyC strategyC;
    void BornPlaceCreate(double & x,double & y,int &tilex,int &tiley);//��ȡҪ���ɵĻ����˵�λ��

public:
    void CreateRobot(int level);         // ���ز�ͬ��ģ

};

#endif
