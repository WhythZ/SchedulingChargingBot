#include "../../../Header/Chargeable/Concrete/Vehicle.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"

Vehicle::Vehicle()
{
	#pragma region SetAnimation
	//��ȡ��������
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Vehicle)->second;

	animIdling.SetLoop(true); animIdling.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharging.SetLoop(true); animCharging.SetAnimFrames(_sheet, 3, 1, { 1 });
	animDischarging.SetLoop(true); animDischarging.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//�޷������ƶ�
	speed = 0;

	//�����£���ʼ������Ϊ0
	currentElectricityRatio = 0.5;
}

void Vehicle::OnUpdate(double _delta)
{
	#pragma region Charing
	static ChargeableManager* _cm = ChargeableManager::Instance();
	//�ڻ����˻��ظ��������
	//if ()
	#pragma endregion

	Chargeable::OnUpdate(_delta);
}