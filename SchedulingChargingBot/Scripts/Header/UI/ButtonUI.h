#ifndef _BUTTON_UI_H_
#define _BUTTON_UI_H_

#include <SDL.h>
#include "../Tilemap/Tile.h"

class ButtonUI
{
private:
	#pragma region Color
	SDL_Color buttonColor = { 255,255,255,255 };
	SDL_Color textColor = { 0,0,0,0 };
	#pragma endregion

	#pragma region TextSize
	SDL_Point strategyTypeTextSize = { 0 };
	#pragma endregion

	#pragma region TextTexture
	SDL_Texture* strategyTypeTextTexture = nullptr;
	#pragma endregion

	SDL_Point buttonSize = { 4 * TILE_SIZE, TILE_SIZE };
	SDL_Rect buttonRect;

public:
	ButtonUI();
	~ButtonUI() = default;

	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif