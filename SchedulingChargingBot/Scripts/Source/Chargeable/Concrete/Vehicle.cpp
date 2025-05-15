#include "../../../Header/Chargeable/Concrete/Vehicle.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"

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

    //车辆为静态，不移动
    speed = 0;
    //初始电量为0
    currentElectricity = 0;
}

void Vehicle::OnUpdate(double _delta)
{
    Chargeable::OnUpdate(_delta);

    #pragma region FullToRemove
    //当车充满电后，就将其移除
    if (!NeedElectricity())
    {
        isValid = false;
        
        //此处可以再加一些离开后的逻辑，比如记录离开的时刻，用作某些分析
    }
    #pragma endregion

    #pragma region ChargedRect
    //可被充电的区域是以车辆为中心的3x3瓦片
    chargedRect.x = (int)(position.x - (TILE_SIZE + TILE_SIZE / 2));
    chargedRect.y = (int)(position.y - (TILE_SIZE + TILE_SIZE / 2));
    chargedRect.w = TILE_SIZE * 3;
    chargedRect.h = TILE_SIZE * 3;
    #pragma endregion
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
