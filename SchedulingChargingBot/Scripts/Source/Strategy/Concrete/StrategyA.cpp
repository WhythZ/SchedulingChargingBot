#include "../../../Header/Strategy/Concrete/StrategyA.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Chargeable/Concrete/Vehicle.h"

static double GetDistance(Vector2 vPos,Vector2 rPos) {
    double dx = vPos.x - rPos.x;
    double dy = vPos.y - rPos.y;
    return std::sqrt(dx * dx + dy * dy);
}

void StrategyA::UpdateMovement(Chargeable* _chargeable)
{
    Robot* robot = dynamic_cast<Robot*>(_chargeable);

    Vector2 robotPos = robot->GetPosition();
    Vector2 PilePos;                              //最近充电桩位置
    PilePos.x = robotPos.x >= 640 ? 1280 : 0;
    PilePos.y = robotPos.y >= 448 ? 896 : 0;
    Vector2 returnPath = PilePos - robotPos;

    //std::cout << returnPath.Length() << std::endl;

    if(!robot->IsBusy())
    {
        Vector2 PileVelocity;
        PileVelocity.x = returnPath.x / returnPath.Length() * robot->GetSpeed();
        PileVelocity.y = returnPath.y / returnPath.Length() * robot->GetSpeed();
        if (returnPath.Length() > 96)
            robot->SetVelocity(PileVelocity);             //初始就设置Robot向充电桩跑
        else
            robot->SetVelocity({ 0,0 });
    }

    // 如果电量为0，直接停止
    if (robot->GetCurrentElectricity() <= 0)
    {
        robot->SetVelocity({ 0, 0 });
        
        return;
    }
    
    if (!robot || robot->IsBusy()) return;

    ChargeableManager* cm = ChargeableManager::Instance();
    const auto& vehicles = cm -> GetVehicleList();

    Vehicle* bT = nullptr;                  //bestTargeted临时存放变量
    double rD = 0.0;                        //returnDistance临时存放变量
    double minDistance = std::numeric_limits<double>::max();

    for (Vehicle* v : vehicles)
    {
        if (v->IsBusy() || !v->NeedElectricity())
            continue;
        Vector2 vPos = v->GetPosition();
        double distance = GetDistance(vPos, robotPos);

        // 可达性检查（足够电量移动到目标与返回充电桩）
        double moveDistance = (distance - 96) + GetDistance(vPos, PilePos);
        if ((robot->GetCurrentElectricity() >= moveDistance / 20) && (distance < minDistance))   //最近距离
        {
            minDistance = distance;
            bT = v;
            rD = GetDistance(vPos, PilePos);
        }
    }

    if (bT)       //bT涓篤ehicle
    {
        if (!bT->isTargeted || (bT->TargetedDistance > minDistance))
        {
            bT->isTargeted = robot;
            bT->TargetedDistance = minDistance;
            if(((Vehicle*)(robot->bestTarget)) != nullptr && ((Vehicle*)(robot->bestTarget)) != bT)
                ((Vehicle*)(robot->bestTarget))->isTargeted = nullptr;
            robot->bestTarget = bT;
            robot->lowestElectricity = rD / 20;
        }
    }
    else
    {
        robot->bestTarget = nullptr;
    }

    if (robot->bestTarget && !robot->isCharger)
    {
        Vector2 dir = (robot->bestTarget)->GetPosition() - robot->GetPosition();
        Vector2 velocity = { 0, 0 };
        velocity.x = dir.x / dir.Length() * robot->GetSpeed();
        velocity.y = dir.y / dir.Length() * robot->GetSpeed();

        robot->SetVelocity(velocity);
        return;
    }
}