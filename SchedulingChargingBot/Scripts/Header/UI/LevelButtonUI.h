#ifndef _LEVEL_BUTTON_UI_H_
#define _LEVEL_BUTTON_UI_H_

#include <SDL.h>
#include "../Tilemap/Tile.h"

class LevelButtonUI
{
private:
    SDL_Color buttonColor = { 200,255,200,255 };        //按钮背景色
    SDL_Color textColor = { 0, 0, 0, 255 };             //文字颜色

    SDL_Point textSize = { 0 };                         //文本纹理尺寸
    SDL_Texture* textTexture = nullptr;                 //文本纹理

    SDL_Point buttonSize = { 4 * TILE_SIZE, TILE_SIZE };
    SDL_Rect buttonRect;

    int spawnLevel = 0;                                 //当前车辆规模等级（0=A，1=B，2=C）

public:
    LevelButtonUI();
    ~LevelButtonUI() = default;

    void OnInput(const SDL_Event&);
    void OnUpdate(SDL_Renderer*);
    void OnRender(SDL_Renderer*);

    int GetSpawnLevel() const { return spawnLevel; }    //可用于调度系统读取当前选择等级
};

#endif
