#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "ResourceManager.h"
#include "../../UI/StatusUI.h"
#include "../../UI/CursorUI.h"
#include "../../UI/RimUI.h"

class UIManager :public Manager<UIManager>
{
	friend class Manager<UIManager>;

private:
	StatusUI* statusUI;
	CursorUI* cursorUI;
	RimUI* rimUI;

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

	//�������϶���λ�á�����ߴ磬��������ͼƬ
	void DrawTexture(SDL_Renderer* , SDL_Texture*, const SDL_Point&, const SDL_Point&);
	void DrawTexture(SDL_Renderer*, TextureResID, const SDL_Point&, const SDL_Point&);
	//�������϶���λ�á����ߴ硢�߿��ȡ�������ɫ��������ɫ��������ֵ���������ƶ�̬���µ���ֵ��
	void DrawDynamicBar(SDL_Renderer*, const SDL_Point&, const SDL_Point&, int, const SDL_Color&, const SDL_Color&, double);
	//�������϶���λ�á�Բ�뾶���߿���ɫ��������ɫ�����ƴ��߿��Բ
	void DrawCircle(SDL_Renderer*, const SDL_Point&, double, const SDL_Color&, const SDL_Color&);
	//�������϶���λ�á����γߴ硢������ɫ������һ������ɫ��
	void DrawBox(SDL_Renderer*, const SDL_Point&, const SDL_Point&, const SDL_Color&);
	//�����������λ�á���ɫ����������������
	void DrawLine(SDL_Renderer*, SDL_Point, SDL_Point, SDL_Color);

private:
	UIManager();
	~UIManager();
};

#endif
