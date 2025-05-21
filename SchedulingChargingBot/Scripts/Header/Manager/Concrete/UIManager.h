#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "ResourceManager.h"
#include "../../UI/StatusUI.h"
#include "../../UI/CursorUI.h"
#include "../../UI/RimUI.h"
#include "../../UI/ButtonUI.h" 

class UIManager :public Manager<UIManager>
{
	friend class Manager<UIManager>;

public:
	SDL_Point cursorPosition = { 0 };       //鼠标指针实际所处的位置

	double textZoomRate = 2;                //文本缩放倍数

private:
	RimUI* rimUI;
	StatusUI* statusUI;
	CursorUI* cursorUI;
	ButtonUI* buttonUI;

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

	//根据左上顶点位置、纹理尺寸，绘制纹理图片
	void DrawTexture(SDL_Renderer* , SDL_Texture*, const SDL_Point&, const SDL_Point&);
	void DrawTexture(SDL_Renderer*, TextureResID, const SDL_Point&, const SDL_Point&);
	//根据左上顶点位置、条尺寸、边框厚度、背景颜色、内容颜色、内容数值比例，绘制动态更新的数值条
	void DrawDynamicBar(SDL_Renderer*, const SDL_Point&, const SDL_Point&, int, const SDL_Color&, const SDL_Color&, double);
	//根据左上顶点位置、圆半径、边框颜色、内容颜色，绘制带边框的圆
	void DrawCircle(SDL_Renderer*, const SDL_Point&, double, const SDL_Color&, const SDL_Color&);
	//根据左上顶点位置、矩形尺寸、矩形颜色，绘制一个矩形色块
	void DrawBox(SDL_Renderer*, const SDL_Rect&, const SDL_Color&);
	void DrawBox(SDL_Renderer*, const SDL_Point&, const SDL_Point&, const SDL_Color&);
	//根据两个点的位置、颜色，绘制两点间的线条
	void DrawLine(SDL_Renderer*, SDL_Point, SDL_Point, SDL_Color);

private:
	UIManager();
	~UIManager();
};

#endif
