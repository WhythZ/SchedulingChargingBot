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
    Vector2 PilePos;                              //������׮λ��
    PilePos.x = robotPos.x >= 640 ? 1280 : 0;
    PilePos.y = robotPos.y >= 448 ? 896 : 0;
    Vector2 returnPath = PilePos - robotPos;

    //std::cout << returnPath.Length() << std::endl;

    if(!robot->IsBusy())
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
    const auto& vehicles = cm -> GetVehicleList();

    Vehicle* bT = nullptr;                  //bestTargeted��ʱ��ű���
    double rD = 0.0;                        //returnDistance��ʱ��ű���
    double minDistance = std::numeric_limits<double>::max();

    for (Vehicle* v : vehicles)
    {
        if (v->IsBusy() || !v->NeedElectricity() || v->isMoving)
            continue;

        Vector2 vPos = v->GetPosition();
        double distance = GetDistance(vPos, robotPos);
        if (v->isTargeted && distance >= v->TargetedDistance)
            continue;

        // �ɴ��Լ�飨�㹻�����ƶ���Ŀ���뷵�س��׮��

        Vector2 vPilePos;                              //��������vehicle������׮λ��
        vPilePos.x = vPos.x >= 640 ? 1280 : 0;
        vPilePos.y = vPos.y >= 448 ? 896 : 0;
        double moveDistance = (distance) + GetDistance(vPos, vPilePos);
        if ((robot->GetCurrentElectricity() >= moveDistance / 30) && (distance < minDistance))   //������룬�ұ����ǲ����ƶ��Ķ��󡣲�Ȼ�ᵼ��bTȷ��ʱ��λ�ú�ʵ��λ�ò�ͬ�������ê��δ��������ȴû�����˳��ĺ���������ҳ����ˡ���������
        {
            minDistance = distance;
            bT = v;
            rD = GetDistance(vPos, vPilePos);
        }
        //if (v->isTargeted->GetCurrentElectricity() == 100 && (robot->GetCurrentElectricity() >= moveDistance / 19) && (distance < minDistance))
        //{   
        //    bT = nullptr;
        //    v->isTargeted = nullptr;
        //    robot->bestTarget = nullptr;
        //    robot->lowestElectricity = 0.0;
        //    minDistance = std::numeric_limits<double>::max();
        //    rD = 0.0;
        //}
    }

    if (bT)
    {
        if (!bT->isTargeted)
        {
            bT->isTargeted = robot;
            bT->TargetedDistance = minDistance;
            if (((Vehicle*)(robot->bestTarget)) != nullptr && ((Vehicle*)(robot->bestTarget)) != bT)
                ((Vehicle*)(robot->bestTarget))->isTargeted = nullptr;
            robot->bestTarget = bT;
            robot->lowestElectricity = rD / 30;
        }
        else if (bT->isTargeted && (bT->TargetedDistance > minDistance))
        {
            if(bT->isTargeted != robot)
            {
				((Robot*)(bT->isTargeted))->bestTarget = nullptr;
                bT->isTargeted = robot;
            }
            bT->TargetedDistance = minDistance;
            if(((Vehicle*)(robot->bestTarget)) != nullptr && ((Vehicle*)(robot->bestTarget)) != bT)
                ((Vehicle*)(robot->bestTarget))->isTargeted = nullptr;
            robot->bestTarget = bT;
            robot->lowestElectricity = rD / 30;
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