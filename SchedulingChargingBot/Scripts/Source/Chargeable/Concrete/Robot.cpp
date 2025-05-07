#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//只有机器人会主动移动，故设置速率大小为正数
	speed = 1;

	//初始化当前电量为最大电量
	currentElectricity = 100;
}

void Robot::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);

	#pragma region LinkVehicle
	static ChargeableManager* _cm = ChargeableManager::Instance();

	//处理与载具的链接
	if (!IsBusy())
	{
		//有电才充电
		if (HasElectricity())
		{
			//遍历所有载具
			for (Vehicle* _vehicle : _cm->GetVehicleList())
			{
				//处于范围内才进行后续判断
				if (IsInRectArea(_vehicle->chargedRect))
				{
					//对方不处于繁忙状态，且需要充电时，才给他充电
					if (!_vehicle->IsBusy() && _vehicle->NeedElectricity())
					{
						//与载具链接
						_cm->TieRobotAndVehicle(this, _vehicle);
						break;
					}
				}
			}
		}
	}

	//处理与载具的分离
	if (IsBusy())
	{
		//若自己没电了，或对方满电了，或脱离区域，则分离
		if (!HasElectricity() || !((Vehicle*)charged)->NeedElectricity() || !IsInRectArea(((Vehicle*)charged)->chargedRect))
			_cm->UntieRobotAndVehicle(this, charged);
	}
	#pragma endregion

	#pragma region LinkStation
	static const std::map<size_t, SDL_Rect>& _stationRects = SceneManager::Instance()->map.GetStationRects();
	
	//只有在未处于繁忙状态时（未与载具链接），处理与充电桩的链接与分离
	if (!IsBusy())
	{
		if (IsInRectsArea(_stationRects))
			ChangeState("Charged");
		else
			ChangeState("Idle");
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

bool Robot::IsBusy() const
{
	return (charged != nullptr);
}