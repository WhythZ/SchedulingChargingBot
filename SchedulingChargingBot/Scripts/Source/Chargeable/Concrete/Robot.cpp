#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//��ȡ��������
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	#pragma region MovementDrainTimer
	//��ȡ���׮Rect����
	static const std::map<size_t, SDL_Rect>& _stationRects = SceneManager::Instance()->map.GetStationRects();

	//ÿ0.05�봥��һ��
	movingDrainTimer.SetOneShot(false);
	movingDrainTimer.SetWaitTime(0.05);
	movingDrainTimer.SetTimeOutTrigger(
		[&]()
		{
			//���������ƶ��ҳ��˳��׮
			if (velocity.Length() > 0.0 && !IsInRectsArea(_stationRects))
			{
				currentElectricity -= 0.25;
				if (currentElectricity < 0.0)
					currentElectricity = 0.0;
			}
		});
	#pragma endregion

	#pragma region LoadConfig
	static ConfigManager* _cm = ConfigManager::Instance();
	speed = _cm->chargeablePrefab.robotSpeed;
	#pragma endregion

	//��ʼ����ǰ����Ϊ������
	currentElectricity = 100.0;
}

void Robot::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);
	movingDrainTimer.OnUpdate(_delta);

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
		if (HasElectricity() && EnoughElectricity())
		{
			//���������ؾ�
			for (Vehicle* _vehicle : _cm->GetVehicleList())
			{
				//���ڷ�Χ�ڲŽ��к����ж�
				if (IsInRectArea(_vehicle->chargedRect))
				{
					//�Է������ڷ�æ״̬,û���ƶ�������Ҫ���ʱ���Ÿ������
					if (!_vehicle->IsBusy() && _vehicle->NeedElectricity() && !_vehicle->rtisMoving())
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
				|| !(((Vehicle*)charged)->GetCurrentElectricity() < ((Vehicle*)charged)->GetTargetElectricity())
				|| !IsInRectArea(((Vehicle*)charged)->chargedRect)
				|| !EnoughElectricity())
			{
				//�����߽��
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

	#pragma region BatteryReact
	for(Battery* _battery : _cm->GetBatteryList())
	{
		//�ڵ�ظ����ҵ������ڵ�أ���������
		if((this->GetCurrentElectricity() < _battery->GetCurrentElectricity())
			&& IsInRectArea(_battery->chargedRect))
			_cm->SwitchElectricityRB(this, _battery);
	}
	#pragma endregion
}

void Robot::ChangeStrategy(Strategy* _strategy)
{
	strategy = _strategy;
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
	else if (_stateName == "Moving")
	{
		isCharged = false;
		isCharger = false;
		isMoving = true;
	}
}

bool Robot::IsBusy() const
{
	return (isCharger || isCharged);
}

bool Robot::EnoughElectricity() const
{
	return (currentElectricity >= lowestElectricity);
}
