#include "../../../Header/Chargeable/Concrete/Vehicle.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"

Vehicle::Vehicle()
{
	#pragma region SetAnimation
	//��ȡ��������
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Vehicle)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//�޷������ƶ�
	speed = 0;

	//��ʼ����ǰ����Ϊ0
	currentElectricityRatio = 0;
}

void Vehicle::OnUpdate(double _delta)
{
	#pragma region Charged
	static ChargeableManager* _cm = ChargeableManager::Instance();
	#pragma endregion

	Chargeable::OnUpdate(_delta);
}