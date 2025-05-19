#include "../../Header/UI/LevelButtonUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"
#include "../../Header/Manager/Concrete/VehicleSpawner.h"

LevelButtonUI::LevelButtonUI()
{
    SDL_Rect mapRect = SceneManager::Instance()->mapRect;
    buttonRect =
    {
        //放在策略按钮右侧
        mapRect.w / 2 + 1 * TILE_SIZE,
        mapRect.h - buttonSize.y,
        buttonSize.x,
        buttonSize.y
    };
    Manager<VehicleSpawner>::Instance()->LoadScenario(spawnLevel);
}

void LevelButtonUI::OnInput(const SDL_Event& event)
{
    static UIManager* ui = UIManager::Instance();
    SDL_Point cursorPos = ui->cursorPosition;

    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
        if (SDL_PointInRect(&cursorPos, &buttonRect))
        {
            // 切换等级 A → B → C → A
            spawnLevel = (spawnLevel + 1) % 3;
            Manager<VehicleSpawner>::Instance()->LoadScenario(spawnLevel);// 调用车辆生成函数
            printf("[UI] Clicked to load scenario level %d\n", spawnLevel);
        }
    }
}

void LevelButtonUI::OnUpdate(SDL_Renderer* renderer)
{
    // 清除前一帧的纹理
    SDL_DestroyTexture(textTexture);
    textTexture = nullptr;

    static TTF_Font* font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;

    std::string textStr = "Level";
    textStr += (spawnLevel == 0 ? "A" : spawnLevel == 1 ? "B" : "C");

    SDL_Surface* surface = TTF_RenderText_Blended(font, textStr.c_str(), textColor);
    textSize = { surface->w, surface->h };
    textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void LevelButtonUI::OnRender(SDL_Renderer* renderer)
{
    static UIManager* ui = UIManager::Instance();
    static double zoom = ui->textZoomRate;

    SDL_Point pos = { buttonRect.x, buttonRect.y };

    // 绘制背景按钮
    ui->DrawBox(renderer, buttonRect, buttonColor);

    // 缩放并绘制文字
    SDL_Point sizeScaled = { int(textSize.x * zoom), int(textSize.y * zoom) };
    pos.x += buttonRect.w / 2 - sizeScaled.x / 2;
    pos.y += buttonRect.h / 2 - sizeScaled.y / 2;

    ui->DrawTexture(renderer, textTexture, pos, sizeScaled);
}
