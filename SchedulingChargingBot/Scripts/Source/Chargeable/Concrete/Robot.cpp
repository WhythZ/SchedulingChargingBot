#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//只有机器人会主动移动，故设置速率大小为正数
	speed = 1;

	//初始化当前电量为最大电量百分比
	currentElectricityRatio = 1;
}

void Robot::OnUpdate(double _delta)
{
	#pragma region Charged
	#pragma endregion

	#pragma region Charger
	static ChargeableManager* _cm = ChargeableManager::Instance();
	#pragma endregion

	Chargeable::OnUpdate(_delta);
}