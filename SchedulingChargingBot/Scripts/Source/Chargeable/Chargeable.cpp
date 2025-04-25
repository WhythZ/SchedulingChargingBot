#include "../../Header/Chargeable/Chargeable.h"
#include <SDL2_gfxPrimitives.h>
#include "../../Header/Tilemap/Tile.h"
#include "../../Header/Manager/Concrete/GameManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

Chargeable::Chargeable()
{
	//��ʼ����ǰ����Ϊ�������ٷֱ�
	currentElectricityRatio = 1;

	//��ʼ����ǰ����Ϊ���ö���
	animCurrent = &animIdling;

	#pragma region SetTimer
	//��ʱ����Ҫ���������Գ��/�ŵ磬ÿ�ζԵ�ǰ��������һ�������ı�
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

	//�趨��ײ�뾶
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
	//�ŵ�����
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
	//��δ����ק���������ٶȸ�����λ��
	static SceneManager* _sm = SceneManager::Instance();
	if (isCursorDragging)
	{
		position.x = (double)(_sm->GetCursorPosition().x);
		position.y = (double)(_sm->GetCursorPosition().y);
	}
	else
		position += velocity * _delta;

	//��ֹ�ܳ���ͼ�߽�
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
	//��������Ⱦ�ڵײ㣬�������ϴ���������������϶��㣨yֵ���ϱ����������¶��㣬Ȼ������ɫ
	static SDL_Rect _barRect;
	//��Ⱦ����������ɫ
	_barRect.w = size.x;
	_barRect.h = size.y;
	_barRect.x = (int)(position.x - size.x / 2);
	_barRect.y = (int)(position.y - size.y / 2);
	boxRGBA(_renderer, _barRect.x, _barRect.y, _barRect.x + _barRect.w, _barRect.y + _barRect.h,
		barBackgroundColor.r, barBackgroundColor.g, barBackgroundColor.b, barBackgroundColor.a);
	//��Ⱦ����������ɫ
	_barRect.w = size.x;
	_barRect.h = (int)(size.y * currentElectricityRatio);
	_barRect.x = (int)(position.x - size.x / 2);
	_barRect.y = (int)(position.y + size.y / 2 - _barRect.h);
	boxRGBA(_renderer, _barRect.x, _barRect.y, _barRect.x + _barRect.w, _barRect.y + _barRect.h,
		barContentColor.r, barContentColor.g, barContentColor.b, barContentColor.a);
	#pragma endregion

	//Ȼ������Ⱦ�����Ը����ڵ�������
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
	//����Ϊ����ʱ����
	animCurrent = &animIdling;
}

void Chargeable::UpdateCharging(double _delta)
{
	//���³��ʱ��ʱ��
	chargingTimer.OnUpdate(_delta);

	//����Ϊ���ʱ����
	animCurrent = &animCharging;
}

void Chargeable::UpdateDischarging(double _delta)
{
	//���·ŵ�ʱ��ʱ��
	dischargingTimer.OnUpdate(_delta);

	//����Ϊ�ŵ�ʱ����
	animCurrent = &animDischarging;
}