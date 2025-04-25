#include "../../Header/Chargeable/Chargeable.h"
#include <SDL2_gfxPrimitives.h>
#include "../../Header/Tilemap/Tile.h"
#include "../../Header/Manager/Concrete/GameManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

Chargeable::Chargeable()
{
	//初始化当前电量为最大电量百分比
	currentElectricityRatio = 1;

	//初始化当前动画为闲置动画
	animCurrent = &animIdling;

	#pragma region SetTimer
	//计时器需要持续更新以充电/放电，每次对当前电量做出一定数量改变
	chargingTimer.SetOneShot(false);
	chargingTimer.SetTimeOutTrigger(
		[&]()
		{
			currentElectricityRatio += chargingIntensity;
			currentElectricityRatio = (currentElectricityRatio >= 1) ? 1 : currentElectricityRatio;
		}
	);

	dischargingTimer.SetOneShot(false);
	dischargingTimer.SetTimeOutTrigger(
		[&]()
		{
			currentElectricityRatio -= dischargingIntensity;
			currentElectricityRatio = (currentElectricityRatio < 0) ? 0 : currentElectricityRatio;
		}
	);
	#pragma endregion

	//设定碰撞半径
	collideRadius = TILE_SIZE;
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

	#pragma region Animation
	//放电优先
	if (isDischarging)
		UpdateDischarging(_delta);
	else if (isCharging)
		UpdateCharging(_delta);
	else
		UpdateIdling(_delta);
	#pragma endregion

	#pragma region Electricity

	#pragma endregion

	#pragma region Position
	//若未被拖拽，则依据速度更新其位置
	static SceneManager* _sm = SceneManager::Instance();
	if (isCursorDragging)
	{
		position.x = (double)(_sm->GetCursorPosition().x);
		position.y = (double)(_sm->GetCursorPosition().y);
	}
	else
		position += velocity * _delta;

	//防止跑出地图边界
	static SDL_Rect _mapRect = _sm->mapRect;
	if (position.x <= _mapRect.x) position.x = _mapRect.x;
	if (position.x >= _mapRect.x + _mapRect.w) position.x = _mapRect.x + _mapRect.w;
	if (position.y <= _mapRect.y) position.y = _mapRect.y;
	if (position.y >= _mapRect.y + _mapRect.h) position.y = _mapRect.y + _mapRect.h;
	#pragma endregion
}

void Chargeable::OnRender(SDL_Renderer* _renderer)
{
	#pragma region ElectricityBar
	//电量条渲染在底层，从下往上代表电量，传入左上顶点（y值乘上比例）和右下顶点，然后是颜色
	static SDL_Rect _barRect;
	//渲染电量背景颜色
	_barRect.w = size.x;
	_barRect.h = size.y;
	_barRect.x = (int)(position.x - size.x / 2);
	_barRect.y = (int)(position.y - size.y / 2);
	boxRGBA(_renderer, _barRect.x, _barRect.y, _barRect.x + _barRect.w, _barRect.y + _barRect.h,
		barBackgroundColor.r, barBackgroundColor.g, barBackgroundColor.b, barBackgroundColor.a);
	//渲染电量内容颜色
	_barRect.w = size.x;
	_barRect.h = (int)(size.y * currentElectricityRatio);
	_barRect.x = (int)(position.x - size.x / 2);
	_barRect.y = (int)(position.y + size.y / 2 - _barRect.h);
	boxRGBA(_renderer, _barRect.x, _barRect.y, _barRect.x + _barRect.w, _barRect.y + _barRect.h,
		barContentColor.r, barContentColor.g, barContentColor.b, barContentColor.a);
	#pragma endregion

	//然后再渲染纹理，以覆盖在电量条上
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