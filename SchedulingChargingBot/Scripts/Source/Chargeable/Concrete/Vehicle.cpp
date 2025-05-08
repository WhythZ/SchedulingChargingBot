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
	currentElectricity = 0;
}

void Vehicle::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);

	#pragma region ChargedRect
	//�����Ա�����������������Ϊ�Գ�Ϊ���ĵ�3x3��Ƭ����
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
	return (isCharged);
}