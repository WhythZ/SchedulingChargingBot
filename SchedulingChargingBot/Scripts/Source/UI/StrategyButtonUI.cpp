#include "../../Header/UI/StrategyButtonUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

StrategyButtonUI::StrategyButtonUI()
{
	//地图Rect
	SDL_Rect _mapRect = SceneManager::Instance()->mapRect;

	//把按钮放在屏幕下方的左侧
	buttonRect =
	{
		_mapRect.w / 2 - buttonSize.x / 2 - 3 * TILE_SIZE,
		_mapRect.h - buttonSize.y,
		buttonSize.x,
		buttonSize.y
	};
}

void StrategyButtonUI::OnInput(const SDL_Event& _event)
{
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static UIManager* _ui = UIManager::Instance();
	SDL_Point _cursorPosition = _ui->cursorPosition;

	switch (_event.type)
	{
	case SDL_MOUSEBUTTONUP:
		//左键按下按钮则切换策略
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			//如果鼠标处于按钮的Rect内
			if (SDL_PointInRect(&_cursorPosition, &buttonRect))
			{
				//切换至下一个策略，循环往复
				switch (_cm->GetRobotStrategyType())
				{
				case StrategyType::A:
					_cm->ChangeStrategy(StrategyType::B);
					break;
				case StrategyType::B:
					_cm->ChangeStrategy(StrategyType::C);
					break;
				case StrategyType::C:
					_cm->ChangeStrategy(StrategyType::A);
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

void StrategyButtonUI::OnUpdate(SDL_Renderer* _renderer)
{
	#pragma region Clear
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(strategyTypeTextTexture);
	strategyTypeTextTexture = nullptr;
	#pragma endregion

	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;
	static ChargeableManager* _cm = ChargeableManager::Instance();

	#pragma region Text
	//转化为字符串
	std::string _strategyTypeStr = "";
	switch (_cm->GetRobotStrategyType())
	{
	case StrategyType::A:
		_strategyTypeStr = "StrategyA";
		break;
	case StrategyType::B:
		_strategyTypeStr = "StrategyB";
		break;
	case StrategyType::C:
		_strategyTypeStr = "StrategyC";
		break;
	default:
		break;
	}
	SDL_Surface* _strategyTypeTextSurface = TTF_RenderText_Blended(_font, _strategyTypeStr.c_str(), textColor);
	//获取转化后的图片的长宽
	strategyTypeTextSize = { _strategyTypeTextSurface->w, _strategyTypeTextSurface->h };
	//然后再将其转化为纹理格式
	strategyTypeTextTexture = SDL_CreateTextureFromSurface(_renderer, _strategyTypeTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_strategyTypeTextSurface);
	#pragma endregion

	//更新按钮的Rect
}

void StrategyButtonUI::OnRender(SDL_Renderer* _renderer)
{
	//引入纹理渲染相关方法
	static UIManager* _ui = UIManager::Instance();
	static double _textZoomRate = _ui->textZoomRate;

	//复用的左上顶点坐标
	static SDL_Point _positionLeftUp = { 0,0 };

	#pragma region Button
	//绘制按钮方框
	_ui->DrawBox(_renderer, buttonRect, buttonColor);
	#pragma endregion

	#pragma region RobotNumText
	//缩放文本大小
	strategyTypeTextSize.x = (int)(strategyTypeTextSize.x * _textZoomRate);
	strategyTypeTextSize.y = (int)(strategyTypeTextSize.y * _textZoomRate);
	//渲染在按钮方框中央
	_positionLeftUp.x = buttonRect.x + buttonRect.w / 2 - strategyTypeTextSize.x / 2;
	_positionLeftUp.y = buttonRect.y + buttonRect.h / 2 - strategyTypeTextSize.y / 2;
	_ui->DrawTexture(_renderer, strategyTypeTextTexture, _positionLeftUp, strategyTypeTextSize);
	#pragma endregion
}