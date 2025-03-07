#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Battery)->second;

	animIdling.SetLoop(true); animIdling.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharging.SetLoop(true); animCharging.SetAnimFrames(_sheet, 3, 1, { 1 });
	animDischarging.SetLoop(true); animDischarging.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion
}