#ifndef _CURSOR_UI_H_
#define _CURSOR_UI_H_

#include <SDL.h>

class CursorUI
{
private:
	SDL_Point cursorTilePosition = { 0 };   //���ָ��������Ƭ�����ĵ�λ��

	bool isTouchChargeable = false;         //��������ʵ����Χ�ڣ�������Ⱦ��Ƭ�������ͣ��

public:
	CursorUI() = default;
	~CursorUI() = default;

	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif
