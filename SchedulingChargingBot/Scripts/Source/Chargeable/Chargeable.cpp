#include "../../Header/Chargeable/Chargeable.h"
#include <SDL2_gfxPrimitives.h>
#include "../../Header/Tilemap/Tile.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/GameManager.h"

Chargeable::Chargeable()
{
	//��ʼ����ǰ����Ϊ������
	currentElectricity = maximumElectricity;

	//��ʼ����ǰ����Ϊ���ö���
	animCurrent = &animIdling;

	#pragma region SetTimer
	//��ʱ����Ҫ���������Գ��/�ŵ磬ÿ�ζԵ�ǰ��������һ�������ı�
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

	#pragma region Position
	//��δ����ק���������ٶȸ�����λ��
	static GameManager* _gm = GameManager::Instance();
	if (isCursorDragging)
	{
		position.x = (double)(_gm->GetCursorPosition().x);
		position.y = (double)(_gm->GetCursorPosition().y);
	}
	else
		position += velocity * _delta;

	//��ֹ�ܳ���ͼ�߽�
	static SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;
	if (position.x <= _mapRect.x) position.x = _mapRect.x;
	if (position.x >= _mapRect.x + _mapRect.w) position.x = _mapRect.x + _mapRect.w;
	if (position.y <= _mapRect.y) position.y = _mapRect.y;
	if (position.y >= _mapRect.y + _mapRect.h) position.y = _mapRect.y + _mapRect.h;
	#pragma endregion
}

void Chargeable::OnRender(SDL_Renderer* _renderer)
{
	#pragma region ElectricityBar
	//�ȼ��㵱ǰ����������������[0,1]֮��
	double _ratio = currentElectricity / maximumElectricity;
	_ratio = (_ratio < 0) ? 0 : _ratio;
	_ratio = (_ratio > 1) ? 1 : _ratio;

	//��������Ⱦ�ڵײ㣬�������ϴ���������������϶��㣨yֵ���ϱ����������¶��㣬Ȼ������ɫ
	static SDL_Rect _barRect;
	_barRect.x = (int)(position.x);
	_barRect.y = (int)(position.y - size.y / 2);
	_barRect.w = size.x / 2;
	_barRect.h = size.y;
	boxRGBA(_renderer, _barRect.x, (Sint16)(_barRect.y * _ratio), _barRect.x + _barRect.w, _barRect.y + _barRect.h,
		barColor.r, barColor.g, barColor.b, barColor.a);
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