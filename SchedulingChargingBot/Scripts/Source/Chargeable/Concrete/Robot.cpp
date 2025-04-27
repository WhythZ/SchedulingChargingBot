#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//��ȡ��������
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//ֻ�л����˻������ƶ������������ʴ�СΪ����
	speed = 1;

	//��ʼ����ǰ����Ϊ�������ٷֱ�
	currentElectricityRatio = 1;
}

void Robot::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);
}

void Robot::ChangeState(std::string _stateName)
{
	if (_stateName == "Idle")
	{
		isCharged = false;
		isCharger = false;
	}
	else if (_stateName == "Charged")
	{
		isCharged = true;
		isCharger = false;
	}
	else if (_stateName == "Charger")
	{
		isCharged = false;
		isCharger = true;
	}
}