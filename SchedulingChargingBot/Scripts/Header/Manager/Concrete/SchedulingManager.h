#ifndef _SCHEDULING_MANAGER_H_
#define _SCHEDULING_MANAGER_H_

#include <SDL.h>
#include "../Manager.hpp"

class SchedulingManager :public Manager<SchedulingManager>
{
	friend class Manager<SchedulingManager>;

private:
	bool isUsingTactic01;
	bool isUsingTactic02;

public:
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

private:
	void Tatic01();
	void Tatic02();
};

#endif