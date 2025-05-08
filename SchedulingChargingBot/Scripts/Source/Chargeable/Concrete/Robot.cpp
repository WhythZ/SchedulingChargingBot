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

	//��ʼ����ǰ����Ϊ������
	currentElectricity = 100;
}

void Robot::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);

	//�ܲ��Կ��ƽ����ƶ�
	strategy->UpdateMovement(this);

	#pragma region ChangeState
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static const std::map<size_t, SDL_Rect>& _stationRects = SceneManager::Instance()->map.GetStationRects();
	//����æ�����ڸ��ؾ߳������ڱ����׮��磩
	if (!IsBusy())
	{
		#pragma region LinkVehicle
		//�е�ų��
		if (HasElectricity())
		{
			//���������ؾ�
			for (Vehicle* _vehicle : _cm->GetVehicleList())
			{
				//���ڷ�Χ�ڲŽ��к����ж�
				if (IsInRectArea(_vehicle->chargedRect))
				{
					//�Է������ڷ�æ״̬������Ҫ���ʱ���Ÿ������
					if (!_vehicle->IsBusy() && _vehicle->NeedElectricity())
					{
						//���ؾ�����
						_cm->TieRobotAndVehicle(this, _vehicle);
						//������к����ж�
						return;
					}
				}
			}
		}
		#pragma endregion

		#pragma region LinkStation
		//�����������򲻷�Χ�ڣ�������
		if (IsInRectsArea(_stationRects) && NeedElectricity())
			ChangeState("Charged");
		#pragma endregion
	}
	//��æʱ�������������ζ���æ��
	else
	{
		#pragma region LinkVehicle
		//�����ڸ��ؾ߳��
		if (isCharger)
		{
			//���Լ�û���ˣ���Է������ˣ����������������
			if (!HasElectricity()
				|| !((Vehicle*)charged)->NeedElectricity()
				|| !IsInRectArea(((Vehicle*)charged)->chargedRect))
			{
				_cm->UntieRobotAndVehicle(this, charged);
				//������к����ж�
				return;
			}
		}
		#pragma endregion

		#pragma region LinkStation
		//�����ڱ����׮���
		if (isCharged)
		{
			//���������򣬻������ˣ������Idle״̬
			if (!IsInRectsArea(_stationRects) || !NeedElectricity())
				ChangeState("Idle");
		}
		#pragma endregion
	}
	#pragma endregion
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

void Robot::ChangeStrategy(Strategy* _strategy)
{
	strategy = _strategy;
}

bool Robot::IsBusy() const
{
	return (isCharger || isCharged);
}