#include "../../Header/Chargeable/Chargeable.h"
#include <SDL2_gfxPrimitives.h>
#include "../../Header/Tilemap/Tile.h"
#include "../../Header/Manager/Concrete/GameManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

Chargeable::Chargeable()
{
	//��ʼ����ǰ����Ϊ���ö���
	animCurrent = &animIdle;

	#pragma region SetTimer
	//��ʱ����Ҫ���������Գ��/�ŵ磬ÿ�ζԵ�ǰ��������һ�������ı�
	chargedTimer.SetOneShot(false);
	chargedTimer.SetWaitTime(chargedCooldown);
	chargedTimer.SetTimeOutTrigger(
		[&]()
		{
			currentElectricity += chargedIntensity;
			currentElectricity = (currentElectricity >= 100) ? 100 : currentElectricity;
		}
	);

	chargerTimer.SetOneShot(false);
	chargerTimer.SetWaitTime(chargerCooldown);
	chargerTimer.SetTimeOutTrigger(
		[&]()
		{
			currentElectricity -= chargerIntensity;
			currentElectricity = (currentElectricity < 0) ? 0 : currentElectricity;
		}
	);
	#pragma endregion
}

void Chargeable::OnUpdate(double _delta)
{
	animCurrent->OnUpdate(_delta);

	#pragma region UpdateByState
	//�ŵ�����
	if (isCharger)
		UpdateCharger(_delta);
	else if (isCharged)
		UpdateCharged(_delta);
	else
		UpdateIdling(_delta);
	#pragma endregion

	#pragma region DragMovement
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
	//��Ⱦ����������ɫ��ע�⸡����100.0
	_barRect.w = size.x;
	_barRect.h = (int)(size.y * (currentElectricity / 100.0));
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

void Chargeable::SetPosition(int _x, int _y)
{
	position.x = _x;
	position.y = _y;
}

void Chargeable::SetVelocity(Vector2 _velocity)
{
	velocity = _velocity;
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

bool Chargeable::HasElectricity() const
{
	return currentElectricity > 0;
}

bool Chargeable::NeedElectricity() const
{
	return currentElectricity < 100;
}

bool Chargeable::IsInRectArea(const SDL_Rect& _rect) const
{
	if (position.x >= _rect.x && position.x <= _rect.x + _rect.w &&
		position.y >= _rect.y && position.y <= _rect.y + _rect.h)
		return true;
	return false;
}

bool Chargeable::IsInRectsArea(const std::map<size_t, SDL_Rect>& _rects) const
{
	bool _flag = false;
	//size_t _idx = 0;
	for (const auto& _iter : _rects)
	{
		const SDL_Rect& _rect = _iter.second;
		if (IsInRectArea(_rect))
		{
			_flag = true;
			break;
		}
	}
	return _flag;
}

void Chargeable::UpdateIdling(double _delta)
{
	//����Ϊ����ʱ����
	animCurrent = &animIdle;
}

void Chargeable::UpdateCharged(double _delta)
{
	//���³��ʱ��ʱ��
	chargedTimer.OnUpdate(_delta);

	//����Ϊ���ʱ����
	animCurrent = &animCharged;
}

void Chargeable::UpdateCharger(double _delta)
{
	//���·ŵ�ʱ��ʱ��
	chargerTimer.OnUpdate(_delta);

	//����Ϊ�ŵ�ʱ����
	animCurrent = &animCharger;
}