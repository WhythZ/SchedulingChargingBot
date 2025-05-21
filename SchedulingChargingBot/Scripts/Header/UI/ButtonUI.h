#ifndef _BUTTON_UI_H_
#define _BUTTON_UI_H_

#include <SDL.h>
#include "../Tilemap/Tile.h"

class ButtonUI
{
private:
	#pragma region Color
	SDL_Color textColor = { 0,0,0,0 };
	SDL_Color buttonColor = { 200,255,200,255 };
	#pragma endregion

	#pragma region TextSize
	SDL_Point strategyTypeTextSize = { 0 };
	SDL_Point levelTypeTextSize = { 0 };
	#pragma endregion

	#pragma region TextTexture
	SDL_Texture* strategyTypeTextTexture = nullptr;
	SDL_Texture* levelTypeTextTexture = nullptr;
	#pragma endregion

	#pragma region ButtonRect
	SDL_Point strategyButtonSize = { 4 * TILE_SIZE, TILE_SIZE };
	SDL_Rect strategyButtonRect;

	SDL_Point levelButtonSize = { 4 * TILE_SIZE, TILE_SIZE };
	SDL_Rect levelButtonRect;
	#pragma endregion

public:
	ButtonUI();
	~ButtonUI() = default;

	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif