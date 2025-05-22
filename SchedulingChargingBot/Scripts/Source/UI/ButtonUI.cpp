#include "../../Header/UI/ButtonUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"
#include "../../Header/Manager/Concrete/SpawnManager.h"

ButtonUI::ButtonUI()
{
	//地图Rect
	SDL_Rect _mapRect = SceneManager::Instance()->mapRect;

	//策略按钮放在屏幕下方的左侧
	strategyButtonRect =
	{
		_mapRect.w / 2 - strategyButtonSize.x / 2 - 3 * TILE_SIZE,
		_mapRect.h - strategyButtonSize.y,
		strategyButtonSize.x,
		strategyButtonSize.y
	};

	//规模等级放在策略按钮右侧
	levelButtonRect =
	{
		_mapRect.w / 2 + 1 * TILE_SIZE,
		_mapRect.h - levelButtonSize.y,
		levelButtonSize.x,
		levelButtonSize.y
	};
}

void ButtonUI::OnInput(const SDL_Event& _event)
{
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static SpawnManager* _sm = SpawnManager::Instance();
	static UIManager* _ui = UIManager::Instance();
	SDL_Point _cursorPosition = _ui->cursorPosition;

	switch (_event.type)
	{
	case SDL_MOUSEBUTTONUP:
		//左键按下按钮则切换策略或规模
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			#pragma region Strategy
			//如果鼠标处于策略按钮的Rect内
			if (SDL_PointInRect(&_cursorPosition, &strategyButtonRect))
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
			#pragma endregion
			
			#pragma region Level
			//如果鼠标处于策略按钮的Rect内
			if (SDL_PointInRect(&_cursorPosition, &levelButtonRect))
			{
				//切换至下一个规模，循环往复
				switch (_sm->GetCurrentLevel())
				{
				case SpawnManager::ScaleLevel::Small:
					//std::cout << "Change Scale Level To Medium\n";
					_sm->ChangeLevel(SpawnManager::ScaleLevel::Medium);
					break;
				case SpawnManager::ScaleLevel::Medium:
					//std::cout << "Change Scale Level To Large\n";
					_sm->ChangeLevel(SpawnManager::ScaleLevel::Large);
					break;
				case SpawnManager::ScaleLevel::Large:
					//std::cout << "Change Scale Level To Small\n";
					_sm->ChangeLevel(SpawnManager::ScaleLevel::Small);
					break;
				}
			}
			#pragma endregion
		}
		break;
	default:
		break;
	}
}

void ButtonUI::OnUpdate(SDL_Renderer* _renderer)
{
	#pragma region Clear
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(strategyTypeTextTexture);
	strategyTypeTextTexture = nullptr;
	SDL_DestroyTexture(levelTypeTextTexture);
	levelTypeTextTexture = nullptr;
	#pragma endregion

	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static SpawnManager* _sm = SpawnManager::Instance();

	#pragma region StrategyText
	//转化为字符串
	std::string _strategyTypeStr = "";
	switch (_cm->GetRobotStrategyType())
	{
	case StrategyType::A:
		_strategyTypeStr = "Strategy=A";
		break;
	case StrategyType::B:
		_strategyTypeStr = "Strategy=B";
		break;
	case StrategyType::C:
		_strategyTypeStr = "Strategy=C";
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

	#pragma region LevelText
	//转化为字符串
	std::string _levelTypeStr = "";
	switch (_sm->GetCurrentLevel())
	{
	case SpawnManager::ScaleLevel::Small:
		_levelTypeStr = "Level=Small";
		break;
	case SpawnManager::ScaleLevel::Medium:
		_levelTypeStr = "Level=Medium";
		break;
	case SpawnManager::ScaleLevel::Large:
		_levelTypeStr = "Level=Large";
		break;
	}
	SDL_Surface* _levelTypeTextSurface = TTF_RenderText_Blended(_font, _levelTypeStr.c_str(), textColor);
	//获取转化后的图片的长宽
	levelTypeTextSize = { _levelTypeTextSurface->w, _levelTypeTextSurface->h };
	//然后再将其转化为纹理格式
	levelTypeTextTexture = SDL_CreateTextureFromSurface(_renderer, _levelTypeTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_levelTypeTextSurface);
	#pragma endregion
}

void ButtonUI::OnRender(SDL_Renderer* _renderer)
{
	//引入纹理渲染相关方法
	static UIManager* _ui = UIManager::Instance();
	static double _textZoomRate = _ui->textZoomRate;

	//复用的左上顶点坐标
	static SDL_Point _positionLeftUp = { 0,0 };

	#pragma region Button
	//绘制按钮方框
	_ui->DrawBox(_renderer, strategyButtonRect, buttonColor);
	_ui->DrawBox(_renderer, levelButtonRect, buttonColor);
	#pragma endregion

	#pragma region Text
	//缩放文本大小
	strategyTypeTextSize.x = (int)(strategyTypeTextSize.x * _textZoomRate);
	strategyTypeTextSize.y = (int)(strategyTypeTextSize.y * _textZoomRate);
	//渲染在按钮方框中央
	_positionLeftUp.x = strategyButtonRect.x + strategyButtonRect.w / 2 - strategyTypeTextSize.x / 2;
	_positionLeftUp.y = strategyButtonRect.y + strategyButtonRect.h / 2 - strategyTypeTextSize.y / 2;
	_ui->DrawTexture(_renderer, strategyTypeTextTexture, _positionLeftUp, strategyTypeTextSize);

	//缩放文本大小
	levelTypeTextSize.x = (int)(levelTypeTextSize.x * _textZoomRate);
	levelTypeTextSize.y = (int)(levelTypeTextSize.y * _textZoomRate);
	//渲染在按钮方框中央
	_positionLeftUp.x = levelButtonRect.x + levelButtonRect.w / 2 - levelTypeTextSize.x / 2;
	_positionLeftUp.y = levelButtonRect.y + levelButtonRect.h / 2 - levelTypeTextSize.y / 2;
	_ui->DrawTexture(_renderer, levelTypeTextTexture, _positionLeftUp, levelTypeTextSize);
	#pragma endregion
}