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
    double timeLeft = std::max(1e-6, v->GetLeaveTime() - currentTime);    // ������Լ�Сֵ
    double energyNeeded = std::max(0.0, v->GetTargetElectricity() - v->GetCurrentElectricity());
    double energyPerSecond = energyNeeded / timeLeft;
    double distanceSum = std::max(1e-9, d2V + d2C);

    return energyPerSecond * (100.0 / distanceSum);                         // ��ֵ�������߽����� + ����������
}

void StrategyB::UpdateMovement(Chargeable* _chargeable)
{
    Robot* robot = dynamic_cast<Robot*>(_chargeable);

    Vector2 robotPos = robot->GetPosition();
    Vector2 PilePos;                                                      //������׮λ��
    PilePos.x = robotPos.x >= 640 ? 1280 : 0;
    PilePos.y = robotPos.y >= 448 ? 896 : 0;
    Vector2 returnPath = PilePos - robotPos;

    //std::cout << returnPath.Length() << std::endl;

    if (!robot->IsBusy())
    {
        Vector2 PileVelocity;
        PileVelocity.x = returnPath.x / returnPath.Length() * robot->GetSpeed();
        PileVelocity.y = returnPath.y / returnPath.Length() * robot->GetSpeed();
        robot->SetVelocity(PileVelocity);             //��ʼ������Robot����׮��
    }

    // �������Ϊ0��ֱ��ֹͣ
    if (robot->GetCurrentElectricity() <= 0)
    {
        robot->SetVelocity({ 0, 0 });

        return;
    }

    if (!robot || robot->IsBusy()) return;

    static ChargeableManager* cm = ChargeableManager::Instance();
    static ScoreManager* sm = ScoreManager::Instance();

    const auto& vehicles = cm->GetVehicleList();

    Vehicle* bT = nullptr;                  // bestTargeted��ʱ��ű���
    double maxValue = -1.0;                 // ����ֵ
    double rD = 0.0;                        // ���س��׮����

    for (Vehicle* v : vehicles) {
        if (v->IsBusy() || !v->NeedElectricity()) continue;

        Vector2 vPos = v->GetPosition();
        double distanceToVehicle = GetDistance(robotPos, vPos);
        double distanceToCharger = GetDistance(vPos, PilePos); // PilePos: ���׮λ��

        // �ɴ��Լ�飨�㹻�����ƶ���Ŀ���뷵�س��׮��
        double moveDistance = (distanceToVehicle - 96) + distanceToCharger;
        if (robot->GetCurrentElectricity() < moveDistance / 20) continue;

        // �����ֵ
        double value = CalculateVehicleValue(v, sm->GetPassTime(), distanceToVehicle, distanceToCharger);
        if (value > maxValue) 
        {
            maxValue = value;
            bT = v;
            rD = distanceToCharger;
        }
        //std::cout << maxValue << std::endl;
    }

    // �������Ŀ��
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