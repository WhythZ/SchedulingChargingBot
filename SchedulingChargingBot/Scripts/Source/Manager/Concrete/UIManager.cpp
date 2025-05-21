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
		//获取实时鼠标指针位置
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
	//临时存储每个渲染元素，每次渲染单个元素时复用
	static SDL_Rect _dstRect;

	//根据传入的顶点与尺寸绘制纹理
	_dstRect = { _LeftUpPosition.x, _LeftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawTexture(SDL_Renderer* _renderer, TextureResID _id, const SDL_Point& _LeftUpPosition, const SDL_Point& _size)
{
	//获取纹理图片资源
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//这个纹理指针不能用static修饰，否则会导致所有调用此函数所绘制出的图形均为第一个调用此函数时的图像
	SDL_Texture* _texture = _texPool.find(_id)->second;

	static SDL_Rect _dstRect;

	_dstRect = { _LeftUpPosition.x, _LeftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawDynamicBar(SDL_Renderer* _renderer, const SDL_Point& _LeftUpPosition, const SDL_Point& _size, int _borderThickness,
	const SDL_Color& _backgroundColor, const SDL_Color& _contentColor, double _contentRatio)
{
	static SDL_Rect _dstRect;

	//先规整传入的比例，控制在[0,1]之间
	double _ratio = (_contentRatio < 0) ? 0 : _contentRatio;
	_ratio = (_ratio > 1) ? 1 : _ratio;

	//数值值条先绘制背景颜色填充，再绘制内容颜色填充，先传入渲染器与左上顶点和右下顶点，然后是颜色
	_dstRect = { _LeftUpPosition.x, _LeftUpPosition.y, _size.x, _size.y };
	boxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	//如果是如下绘制圆角矩形，则还需传入圆角半径
	//roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
	//	4, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

	//绘制内容的填充矩形（其水平宽度按照比例实时更新），其依据边框宽度收窄，背景颜色就相当于在内容颜色的外围形成一圈边框
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
	//绘制圆填充
	filledCircleRGBA(_renderer, _LeftUpPosition.x, _LeftUpPosition.y, (Sint16)_radius,
		_contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
	//绘制圆边框
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
	//保存当前颜色
	Uint8 _prevR, _prevG, _prevB, _prevA;
	SDL_GetRenderDrawColor(_renderer, &_prevR, &_prevG, &_prevB, &_prevA);

	//设置新颜色
	SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);

	//绘制线条
	SDL_RenderDrawLine(_renderer, _beginPoint.x, _beginPoint.y, _endPoint.x, _endPoint.y);

	//恢复之前的颜色
	SDL_SetRenderDrawColor(_renderer, _prevR, _prevG, _prevB, _prevA);
}