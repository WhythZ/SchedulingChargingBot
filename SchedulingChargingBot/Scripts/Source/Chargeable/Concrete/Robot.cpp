#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdling.SetLoop(true); animIdling.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharging.SetLoop(true); animCharging.SetAnimFrames(_sheet, 3, 1, { 1 });
	animDischarging.SetLoop(true); animDischarging.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//只有机器人会主动移动，故设置速率大小为正数
	speed = 1;
}

void Robot::OnUpdate(double _delta)
{
	#pragma region Discharging
	static ChargeableManager* _cm = ChargeableManager::Instance();
	////若处于非繁忙状态才可以进入新的充放电状态
	//if (!isCharging && !isDischarging)
	//{
	//	//在车辆附近放电
	//	for (Vehicle* _vehicle : _cm->GetVehicleList())
	//	{
	//		if ((position - _vehicle->GetPosition()).Length() <= collideRadius)
	//		{
	//			isDischarging = true;
	//			_vehicle->isCharging = true;
	//			linkEntity = _vehicle;
	//			_vehicle->linkEntity = this;
	//		}
	//	}
	//}
	#pragma endregion

	#pragma region Charing
	//在充电桩附近被充电
	//if ()
	#pragma endregion

	Chargeable::OnUpdate(_delta);
}