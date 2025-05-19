#ifndef _LEVEL_BUTTON_UI_H_
#define _LEVEL_BUTTON_UI_H_

#include <SDL.h>
#include "../Tilemap/Tile.h"

class LevelButtonUI
{
private:
    SDL_Color buttonColor = { 200,255,200,255 };        //��ť����ɫ
    SDL_Color textColor = { 0, 0, 0, 255 };             //������ɫ

    SDL_Point textSize = { 0 };                         //�ı�����ߴ�
    SDL_Texture* textTexture = nullptr;                 //�ı�����

    SDL_Point buttonSize = { 4 * TILE_SIZE, TILE_SIZE };
    SDL_Rect buttonRect;

    int spawnLevel = 0;                                 //��ǰ������ģ�ȼ���0=A��1=B��2=C��

public:
    LevelButtonUI();
    ~LevelButtonUI() = default;

    void OnInput(const SDL_Event&);
    void OnUpdate(SDL_Renderer*);
    void OnRender(SDL_Renderer*);

    int GetSpawnLevel() const { return spawnLevel; }    //�����ڵ���ϵͳ��ȡ��ǰѡ��ȼ�
};

#endif
