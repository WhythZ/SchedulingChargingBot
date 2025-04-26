#ifndef _RIM_UI_H_
#define _RIM_UI_H_

#include <vector>
#include <SDL.h>

class RimUI
{
public:
	RimUI() = default;
	~RimUI() = default;

	//void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif