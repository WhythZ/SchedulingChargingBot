#include "../../../Header/Strategy/Concrete/StrategyA.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ScoreManager.h"
#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Chargeable/Concrete/Vehicle.h"

static double GetDistance(Vector2 vPos, Vector2 rPos) {
    double dx = vPos.x - rPos.x;
    double dy = vPos.y - rPos.y;
    return std::sqrt(dx * dx + dy * dy);
}

static double CalculateVehicleValue(Vehicle* v, double currentTime, double d2V, double d2C) {
    double timeLeft = std::max(1e-6, v->GetLeaveTime() - currentTime);    // 避免除以极小值
    double energyNeeded = std::max(0.0, v->GetTargetElectricity() - v->GetCurrentElectricity());
    double energyPerSecond = energyNeeded / timeLeft;
    double distanceSum = std::max(1e-9, d2V + d2C);

    return energyPerSecond * (100.0 / distanceSum);                         // 价值函数：高紧急度 + 短往返距离
}

void StrategyB::UpdateMovement(Chargeable* _chargeable)
{
    Robot* robot = dynamic_cast<Robot*>(_chargeable);

    Vector2 robotPos = robot->GetPosition();
    Vector2 PilePos;                                                      //最近充电桩位置
    PilePos.x = robotPos.x >= 640 ? 1280 : 0;
    PilePos.y = robotPos.y >= 448 ? 896 : 0;
    Vector2 returnPath = PilePos - robotPos;

    //std::cout << returnPath.Length() << std::endl;

    if (!robot->IsBusy())
    {
        Vector2 PileVelocity;
        PileVelocity.x = returnPath.x / returnPath.Length() * robot->GetSpeed();
        PileVelocity.y = returnPath.y / returnPath.Length() * robot->GetSpeed();
        robot->SetVelocity(PileVelocity);             //初始就设置Robot向充电桩跑
    }

    // 如果电量为0，直接停止
    if (robot->GetCurrentElectricity() <= 0)
    {
        robot->SetVelocity({ 0, 0 });

        return;
    }

    if (!robot || robot->IsBusy()) return;

    static ChargeableManager* cm = ChargeableManager::Instance();
    static ScoreManager* sm = ScoreManager::Instance();

    const auto& vehicles = cm->GetVehicleList();

    Vehicle* bT = nullptr;                  // bestTargeted临时存放变量
    double maxValue = -1.0;                 // 最大价值
    double rD = 0.0;                        // 返回充电桩距离

    for (Vehicle* v : vehicles) {
        if (v->IsBusy() || !v->NeedElectricity()) continue;

        Vector2 vPos = v->GetPosition();
        double distanceToVehicle = GetDistance(robotPos, vPos);
        double distanceToCharger = GetDistance(vPos, PilePos); // PilePos: 充电桩位置

        // 可达性检查（足够电量移动到目标与返回充电桩）
        double moveDistance = (distanceToVehicle - 96) + distanceToCharger;
        if (robot->GetCurrentElectricity() < moveDistance / 20) continue;

        // 计算价值
        double value = CalculateVehicleValue(v, sm->GetPassTime(), distanceToVehicle, distanceToCharger);
        if (value > maxValue) 
        {
            maxValue = value;
            bT = v;
            rD = distanceToCharger;
        }
        //std::cout << maxValue << std::endl;
    }

    // 设置最佳目标
    if (bT) {
        if (!bT->isTargeted) {
            bT->isTargeted = robot;
			bT->TargetedValue = maxValue;
            if (((Vehicle*)(robot->bestTarget)) != nullptr && ((Vehicle*)(robot->bestTarget)) != bT)
                ((Vehicle*)(robot->bestTarget))->isTargeted = nullptr;
            robot->bestTarget = bT;
            robot->lowestElectricity = rD / 20;
        }
        else if (bT->isTargeted && (bT->TargetedValue < maxValue)) {
            if (bT->isTargeted != robot) {
                ((Robot*)(bT->isTargeted))->bestTarget = nullptr;
                bT->isTargeted = robot;
            }
            bT->TargetedValue = maxValue;
            if (((Vehicle*)(robot->bestTarget)) != nullptr && ((Vehicle*)(robot->bestTarget)) != bT)
                ((Vehicle*)(robot->bestTarget))->isTargeted = nullptr;
            robot->bestTarget = bT;
            robot->lowestElectricity = rD / 20;
        }
    }
    else {
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