#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"

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

void Robot::UpdateState(double _delta)
{
	static const Map& _map = SceneManager::Instance()->map;
	std::map<size_t, SDL_Rect> _stationRects = _map.GetStationRects();
	std::map<size_t, SDL_Rect> _vehicleRects = _map.GetVehicleRects();

	//�����ڳ��׮��������
	if (IsInRectsArea(_stationRects))
	{
		isCharged = true;
		isCharger = false;
	}
	else
		isCharged = false;
	//�����ڳ�����Χ����ŵ�
	if (IsInRectsArea(_vehicleRects))
	{
		isCharged = false;
		isCharger = true;
	}
	else
		isCharger = false;
}