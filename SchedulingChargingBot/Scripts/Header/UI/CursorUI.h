#ifndef _CURSOR_UI_H_
#define _CURSOR_UI_H_

#include <SDL.h>

class CursorUI
{
private:
	SDL_Point cursorTilePosition = { 0 };   //鼠标指针所处瓦片的中心点位置

	bool isTouchChargeable = false;         //如果鼠标在实例范围内，无需渲染瓦片的鼠标悬停框

public:
	CursorUI() = default;
	~CursorUI() = default;

	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif
