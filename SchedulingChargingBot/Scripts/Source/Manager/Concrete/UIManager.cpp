#include "../../../Header/Manager/Concrete/UIManager.h"
#include <iostream>
#include <SDL2_gfxPrimitives.h>
#include "../../../Header/Manager/Concrete/GameManager.h"

UIManager::UIManager()
{
	rimUI = new RimUI();
	cursorUI = new CursorUI();
	statusUI = new StatusUI();
	buttonUI = new ButtonUI();
}

UIManager::~UIManager()
{
	delete rimUI;
	delete cursorUI;
	delete statusUI;
	delete buttonUI;
}

void UIManager::OnInput(const SDL_Event& _event)
{
	#pragma region CursorPosition
	switch (_event.type)
	{
	case SDL_MOUSEMOTION:
	{
		//��ȡʵʱ���ָ��λ��
		cursorPosition.x = _event.motion.x;
		cursorPosition.y = _event.motion.y;
	}
	default:
		break;
	};
	#pragma endregion

	cursorUI->OnInput(_event);
	buttonUI->OnInput(_event);
}

void UIManager::OnUpdate(SDL_Renderer* _renderer)
{
	cursorUI->OnUpdate(_renderer);
	statusUI->OnUpdate(_renderer);
	buttonUI->OnUpdate(_renderer);
}

void UIManager::OnRender(SDL_Renderer* _renderer)
{
	rimUI->OnRender(_renderer);
	cursorUI->OnRender(_renderer);
	statusUI->OnRender(_renderer);
	buttonUI->OnRender(_renderer);
}

void UIManager::DrawTexture(SDL_Renderer* _renderer, SDL_Texture* _texture, const SDL_Point& _LeftUpPosition, const SDL_Point& _size)
{
	//��ʱ�洢ÿ����ȾԪ�أ�ÿ����Ⱦ����Ԫ��ʱ����
	static SDL_Rect _dstRect;

	//���ݴ���Ķ�����ߴ��������
	_dstRect = { _LeftUpPosition.x, _LeftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawTexture(SDL_Renderer* _renderer, TextureResID _id, const SDL_Point& _LeftUpPosition, const SDL_Point& _size)
{
	//��ȡ����ͼƬ��Դ
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//�������ָ�벻����static���Σ�����ᵼ�����е��ô˺��������Ƴ���ͼ�ξ�Ϊ��һ�����ô˺���ʱ��ͼ��
	SDL_Texture* _texture = _texPool.find(_id)->second;

	static SDL_Rect _dstRect;

	_dstRect = { _LeftUpPosition.x, _LeftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawDynamicBar(SDL_Renderer* _renderer, const SDL_Point& _LeftUpPosition, const SDL_Point& _size, int _borderThickness,
	const SDL_Color& _backgroundColor, const SDL_Color& _contentColor, double _contentRatio)
{
	static SDL_Rect _dstRect;

	//�ȹ�������ı�����������[0,1]֮��
	double _ratio = (_contentRatio < 0) ? 0 : _contentRatio;
	_ratio = (_ratio > 1) ? 1 : _ratio;

	//��ֵֵ���Ȼ��Ʊ�����ɫ��䣬�ٻ���������ɫ��䣬�ȴ�����Ⱦ�������϶�������¶��㣬Ȼ������ɫ
	_dstRect = { _LeftUpPosition.x, _LeftUpPosition.y, _size.x, _size.y };
	boxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	//��������»���Բ�Ǿ��Σ����贫��Բ�ǰ뾶
	//roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
	//	4, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

	//�������ݵ������Σ���ˮƽ��Ȱ��ձ���ʵʱ���£��������ݱ߿�����խ��������ɫ���൱����������ɫ����Χ�γ�һȦ�߿�
	_dstRect = { _LeftUpPosition.x + _borderThickness, _LeftUpPosition.y + _borderThickness,
		(int)((_size.x - 2 * _borderThickness) * _ratio), _size.y - 2 * _borderThickness };
	boxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		_contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
	//roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
	//	4, _contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
}

void UIManager::DrawCircle(SDL_Renderer* _renderer, const SDL_Point& _LeftUpPosition, double _radius,
	const SDL_Color& _borderColor, const SDL_Color& _contentColor)
{
	//����Բ���
	filledCircleRGBA(_renderer, _LeftUpPosition.x, _LeftUpPosition.y, (Sint16)_radius,
		_contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
	//����Բ�߿�
	aacircleRGBA(_renderer, _LeftUpPosition.x, _LeftUpPosition.y, (Sint16)_radius,
		_borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
}

void UIManager::DrawBox(SDL_Renderer* _renderer, const SDL_Rect& _rect, const SDL_Color& _color)
{
	boxRGBA(_renderer, _rect.x, _rect.y, _rect.x + _rect.w, _rect.y + _rect.h,
		_color.r, _color.g, _color.b, _color.a);
}

void UIManager::DrawBox(SDL_Renderer* _renderer, const SDL_Point& _LeftUpPosition, const SDL_Point& _size, const SDL_Color& _color)
{
	boxRGBA(_renderer, _LeftUpPosition.x, _LeftUpPosition.y, _LeftUpPosition.x + _size.x, _LeftUpPosition.y + _size.y,
		_color.r, _color.g, _color.b, _color.a);
}

void UIManager::DrawLine(SDL_Renderer* _renderer, SDL_Point _beginPoint, SDL_Point _endPoint, SDL_Color _color)
{
	//���浱ǰ��ɫ
	Uint8 _prevR, _prevG, _prevB, _prevA;
	SDL_GetRenderDrawColor(_renderer, &_prevR, &_prevG, &_prevB, &_prevA);

	//��������ɫ
	SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);

	//��������
	SDL_RenderDrawLine(_renderer, _beginPoint.x, _beginPoint.y, _endPoint.x, _endPoint.y);

	//�ָ�֮ǰ����ɫ
	SDL_SetRenderDrawColor(_renderer, _prevR, _prevG, _prevB, _prevA);
}