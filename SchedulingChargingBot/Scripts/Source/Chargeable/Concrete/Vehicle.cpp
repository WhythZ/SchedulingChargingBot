#include "../../../Header/Chargeable/Concrete/Vehicle.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include<iostream>

Vehicle::Vehicle()
{
    #pragma region SetAnimation
    //获取静态资源中的车辆动画图集
    static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Vehicle)->second;

    //设置三种动画状态
    animIdle.SetLoop(true);    animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
    animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
    animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
    #pragma endregion

    //初始电量为0
    currentElectricity = 0;
}

void Vehicle::OnUpdate(double _delta)
{
    Chargeable::OnUpdate(_delta);

    #pragma region ChargedRect
    //可被充电的区域是以车辆为中心的3x3瓦片
    chargedRect.x = (int)(position.x - (TILE_SIZE + TILE_SIZE / 2));
    chargedRect.y = (int)(position.y - (TILE_SIZE + TILE_SIZE / 2));
    chargedRect.w = TILE_SIZE * 3;
    chargedRect.h = TILE_SIZE * 3;
    #pragma endregion
    //std::cout << this->GetCurrentElectricity() << "\n";
}

void Vehicle::ChangeState(std::string _stateName)
{
    if (_stateName == "Idle")
        isCharged = false;
    else if (_stateName == "Charged")
        isCharged = true;
}

bool Vehicle::IsBusy() const
{
    return isCharged;
}

bool Vehicle::NeedElectricity() const
{
    return currentElectricity < targetElectricity;
}

void Vehicle::SetElectricity(double e)
{
    currentElectricity = e;
}

double Vehicle::GetElectricity() const
{
    return currentElectricity;
}

void Vehicle::SetTargetElectricity(double e)
{
    targetElectricity = e;
}

double Vehicle::GetTargetElectricity() const
{
    return targetElectricity;
}

void Vehicle::SetLeaveTime(double t)
{
    leaveTime = t;
}

double Vehicle::GetLeaveTime() const
{
    return leaveTime;
}