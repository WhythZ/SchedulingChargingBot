#ifndef _RIM_UI_H_
#define _RIM_UI_H_

#include <vector>
#include <SDL.h>

class RimUI
{
private:
	std::vector<SDL_Rect> stationRects;
	std::vector<SDL_Rect> vehicleRects;

public:
	RimUI();
	~RimUI() = default;

	//void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif