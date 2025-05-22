#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	#pragma region MovementDrainTimer
	//获取充电桩Rect数据
	static const std::map<size_t, SDL_Rect>& _stationRects = SceneManager::Instance()->map.GetStationRects();

	//每0.05秒触发一次
	movingDrainTimer.SetOneShot(false);
	movingDrainTimer.SetWaitTime(0.05);
	movingDrainTimer.SetTimeOutTrigger(
		[&]()
		{
			//仅当正在移动且出了充电桩
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

	//初始化当前电量为最大电量
	currentElectricity = 100.0;
}

void Robot::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);
	movingDrainTimer.OnUpdate(_delta);

	//受策略控制进行移动
	strategy->UpdateMovement(this);

	#pragma region ChangeState
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static const std::map<size_t, SDL_Rect>& _stationRects = SceneManager::Instance()->map.GetStationRects();
	//不繁忙（正在给载具充电或正在被充电桩充电）
	if (!IsBusy())
	{
		#pragma region LinkVehicle
		//有电才充电
		if (HasElectricity() && EnoughElectricity())
		{
			//遍历所有载具
			for (Vehicle* _vehicle : _cm->GetVehicleList())
			{
				//处于范围内才进行后续判断
				if (IsInRectArea(_vehicle->chargedRect))
				{
					//对方不处于繁忙状态,没在移动，且需要充电时，才给他充电
					if (!_vehicle->IsBusy() && _vehicle->NeedElectricity() && !_vehicle->rtisMoving())
					{
						//与载具链接
						_cm->TieRobotAndVehicle(this, _vehicle);
						//无需进行后续判断
						return;
					}
				}
			}
		}
		#pragma endregion

		#pragma region LinkStation
		//电量不满，或不范围内，才需充电
		if (IsInRectsArea(_stationRects) && NeedElectricity())
			ChangeState("Charged");
		#pragma endregion
	}
	//繁忙时（需分类讨论因何而繁忙）
	else
	{
		#pragma region LinkVehicle
		//若正在给载具充电
		if (isCharger)
		{
			//若自己没电了，或对方满电了，或脱离区域，则分离
			if (!HasElectricity()
				|| !(((Vehicle*)charged)->GetCurrentElectricity() < ((Vehicle*)charged)->GetTargetElectricity())
				|| !IsInRectArea(((Vehicle*)charged)->chargedRect)
				|| !EnoughElectricity())
			{
				//将二者解绑
				_cm->UntieRobotAndVehicle(this, charged);
				//无需进行后续判断
				return;
			}
		}
		#pragma endregion

		#pragma region LinkStation
		//若正在被充电桩充电
		if (isCharged)
		{
			//若脱离区域，或满电了，则进入Idle状态
			if (!IsInRectsArea(_stationRects) || !NeedElectricity())
				ChangeState("Idle");
		}
		#pragma endregion
	}
	#pragma endregion

	#pragma region BatteryReact
	for(Battery* _battery : _cm->GetBatteryList())
	{
		//在电池附近且电量少于电池，交换电量
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
