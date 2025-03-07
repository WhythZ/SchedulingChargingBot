#include "../../../Header/Chargeable/Concrete/Battery.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/VehicleManager.h"

Battery::Battery()
{
	#pragma region SetAnimation
	//��ȡ��������
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Battery)->second;

	animIdling.SetLoop(true); animIdling.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharging.SetLoop(true); animCharging.SetAnimFrames(_sheet, 3, 1, { 1 });
	animDischarging.SetLoop(true); animDischarging.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//�޷������ƶ�
	speed = 0;
}

void Battery::OnUpdate(double _delta)
{
	#pragma region Discharging
	static VehicleManager* _vm = VehicleManager::Instance();
	//�ڳ��������ŵ�
	//if ()
	#pragma endregion

	#pragma region Charing
	//�ڳ��׮���������
	//if ()
	#pragma endregion

	Chargeable::OnUpdate(_delta);
}