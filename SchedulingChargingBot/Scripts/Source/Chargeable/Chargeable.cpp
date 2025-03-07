#include "../../Header/Chargeable/Chargeable.h"
#include "../../Header/Tilemap/Tile.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/GameManager.h"

Chargeable::Chargeable()
{
	//初始化当前电量为最大电量
	currentElectricity = maximumElectricity;

	//初始化当前动画为闲置动画
	animCurrent = &animIdling;

	#pragma region SetTimer
	//计时器需要持续更新以充电/放电，每次对当前电量做出一定数量改变
	chargingTimer.SetOneShot(false);
	chargingTimer.SetTimeOutTrigger(
		[&]()
		{
			currentElectricity += chargingIntensity;
			currentElectricity = (currentElectricity > maximumElectricity) ? maximumElectricity : currentElectricity;
		}
	);

	dischargingTimer.SetOneShot(false);
	dischargingTimer.SetTimeOutTrigger(
		[&]()
		{
			currentElectricity -= dischargingIntensity;
			currentElectricity = (currentElectricity < 0) ? 0 : currentElectricity;
		}
	);
	#pragma endregion
}

void Chargeable::SetPosition(int _x, int _y)
{
	position.x = _x;
	position.y = _y;
}

void Chargeable::SetVelocity(Vector2 _velocity)
{
	velocity = _velocity;
}


void Chargeable::OnUpdate(double _delta)
{
	animCurrent->OnUpdate(_delta);

	if (isCharging)
		UpdateCharging(_delta);
	else if (isDischarging)
		UpdateDischarging(_delta);
	else
		UpdateIdling(_delta);

	//若未被拖拽，则依据速度更新其位置
	static GameManager* _gm = GameManager::Instance();
	if (isCursorDragging)
	{
		position.x = (double)(_gm->GetCursorPosition().x);
		position.y = (double)(_gm->GetCursorPosition().y);
	}
	else
		position += velocity * _delta;

	//防止跑出地图边界
	static SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;
	if (position.x <= _mapRect.x) position.x = _mapRect.x;
	if (position.x >= _mapRect.x + _mapRect.w) position.x = _mapRect.x + _mapRect.w;
	if (position.y <= _mapRect.y) position.y = _mapRect.y;
	if (position.y >= _mapRect.y + _mapRect.h) position.y = _mapRect.y + _mapRect.h;
}

void Chargeable::OnRender(SDL_Renderer* _renderer)
{
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);
	animCurrent->OnRender(_renderer, _point);
}

void Chargeable::Invalidate()
{
	isValid = false;
}

bool Chargeable::IsValid() const
{
	return isValid;
}

Vector2 Chargeable::GetPosition() const
{
	return position;
}

void Chargeable::UpdateIdling(double _delta)
{
	//更新为闲置时动画
	animCurrent = &animIdling;
}

void Chargeable::UpdateCharging(double _delta)
{
	//更新充电时计时器
	chargingTimer.OnUpdate(_delta);

	//更新为充电时动画
	animCurrent = &animCharging;
}

void Chargeable::UpdateDischarging(double _delta)
{
	//更新放电时计时器
	dischargingTimer.OnUpdate(_delta);

	//更新为放电时动画
	animCurrent = &animDischarging;
}