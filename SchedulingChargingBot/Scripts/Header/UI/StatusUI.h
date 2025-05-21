#ifndef _STATUS_UI_
#define _STATUS_UI_

#include <SDL.h>

class StatusUI
{
private:
	#pragma region Color
	SDL_Color textColorNormal = { 0,0,0,0 };
	SDL_Color textColorSpecial = { 127,0,127,0 };
	#pragma endregion

	#pragma region Distance
	int rowDistance = 2;
	#pragma endregion

	#pragma region TextSize
	SDL_Point robotNumTextSize = { 0 };
	SDL_Point vehicleNumTextSize = { 0 };
	SDL_Point batteryNumTextSize = { 0 };
	SDL_Point timeTextSize = { 0 };
	SDL_Point hitTextSize = { 0 };
	SDL_Point missTextSize = { 0 };
	#pragma endregion

	#pragma region TextTexture
	SDL_Texture* robotNumTextTexture = nullptr;
	SDL_Texture* vehicleNumTextTexture = nullptr;
	SDL_Texture* batteryNumTextTexture = nullptr;
	SDL_Texture* timeTextTexture = nullptr;
	SDL_Texture* hitTextTexture = nullptr;
	SDL_Texture* missTextTexture = nullptr;
	#pragma endregion

public:
	StatusUI() = default;
	~StatusUI() = default;

	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif
