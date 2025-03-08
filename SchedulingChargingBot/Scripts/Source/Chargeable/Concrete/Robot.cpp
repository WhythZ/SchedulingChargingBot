#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//��ȡ��������
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdling.SetLoop(true); animIdling.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharging.SetLoop(true); animCharging.SetAnimFrames(_sheet, 3, 1, { 1 });
	animDischarging.SetLoop(true); animDischarging.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//ֻ�л����˻������ƶ������������ʴ�СΪ����
	speed = 1;
}

void Robot::OnUpdate(double _delta)
{
	#pragma region Discharging
	static ChargeableManager* _cm = ChargeableManager::Instance();
	////�����ڷǷ�æ״̬�ſ��Խ����µĳ�ŵ�״̬
	//if (!isCharging && !isDischarging)
	//{
	//	//�ڳ��������ŵ�
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
	//�ڳ��׮���������
	//if ()
	#pragma endregion

	Chargeable::OnUpdate(_delta);
}