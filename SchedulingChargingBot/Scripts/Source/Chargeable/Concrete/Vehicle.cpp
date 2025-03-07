#include "../../../Header/Chargeable/Concrete/Vehicle.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/RobotManager.h"
#include "../../../Header/Manager/Concrete/BatteryManager.h"

Vehicle::Vehicle()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Vehicle)->second;

	animIdling.SetLoop(true); animIdling.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharging.SetLoop(true); animCharging.SetAnimFrames(_sheet, 3, 1, { 1 });
	animDischarging.SetLoop(true); animDischarging.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//无法主动移动
	speed = 0;

	//（重新）初始化电量为0
	currentElectricityRatio = 0.5;
}

void Vehicle::OnUpdate(double _delta)
{
	#pragma region Charing
	static RobotManager* _rm = RobotManager::Instance();
	static BatteryManager* _bm = BatteryManager::Instance();
	//在机器人或电池附近被充电
	//if ()
	#pragma endregion

	Chargeable::OnUpdate(_delta);
}