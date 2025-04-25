#ifndef _STATUS_UI_
#define _STATUS_UI_

#include <SDL.h>

class StatusUI
{
private:
	#pragma region Color
	SDL_Color textColor = { 255,255,255,255 };
	#pragma endregion

	#pragma region Distance
	int rowDistance = 3;
	#pragma endregion

	#pragma region TextSize
	SDL_Point robotNumTextSize = { 0 };
	SDL_Point vehicleNumTextSize = { 0 };

	double textZoomRate = 2;
	#pragma endregion

	#pragma region TextTexture
	SDL_Texture* robotNumTextTexture = nullptr;
	SDL_Texture* vehicleNumTextTexture = nullptr;
	#pragma endregion

public:
	StatusUI() = default;
	~StatusUI() = default;

	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif
