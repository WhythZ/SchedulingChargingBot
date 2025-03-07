#ifndef _LINK_UI_H_
#define _LINK_UI_H_

#include <SDL.h>

class LinkUI
{
public:
	LinkUI() = default;
	~LinkUI() = default;

	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif