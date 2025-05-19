#include "../../Header/UI/StrategyButtonUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

StrategyButtonUI::StrategyButtonUI()
{
	//��ͼRect
	SDL_Rect _mapRect = SceneManager::Instance()->mapRect;

	//�Ѱ�ť������Ļ�·������
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
		//������°�ť���л�����
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			//�����괦�ڰ�ť��Rect��
			if (SDL_PointInRect(&_cursorPosition, &buttonRect))
			{
				//�л�����һ�����ԣ�ѭ������
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
	//����ÿ֡OnUpdate������������һ���ı������������������һ֡����������
	SDL_DestroyTexture(strategyTypeTextTexture);
	strategyTypeTextTexture = nullptr;
	#pragma endregion

	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;
	static ChargeableManager* _cm = ChargeableManager::Instance();

	#pragma region Text
	//ת��Ϊ�ַ���
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
	//��ȡת�����ͼƬ�ĳ���
	strategyTypeTextSize = { _strategyTypeTextSurface->w, _strategyTypeTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	strategyTypeTextTexture = SDL_CreateTextureFromSurface(_renderer, _strategyTypeTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_strategyTypeTextSurface);
	#pragma endregion

	//���°�ť��Rect
}

void StrategyButtonUI::OnRender(SDL_Renderer* _renderer)
{
	//����������Ⱦ��ط���
	static UIManager* _ui = UIManager::Instance();
	static double _textZoomRate = _ui->textZoomRate;

	//���õ����϶�������
	static SDL_Point _positionLeftUp = { 0,0 };

	#pragma region Button
	//���ư�ť����
	_ui->DrawBox(_renderer, buttonRect, buttonColor);
	#pragma endregion

	#pragma region RobotNumText
	//�����ı���С
	strategyTypeTextSize.x = (int)(strategyTypeTextSize.x * _textZoomRate);
	strategyTypeTextSize.y = (int)(strategyTypeTextSize.y * _textZoomRate);
	//��Ⱦ�ڰ�ť��������
	_positionLeftUp.x = buttonRect.x + buttonRect.w / 2 - strategyTypeTextSize.x / 2;
	_positionLeftUp.y = buttonRect.y + buttonRect.h / 2 - strategyTypeTextSize.y / 2;
	_ui->DrawTexture(_renderer, strategyTypeTextTexture, _positionLeftUp, strategyTypeTextSize);
	#pragma endregion
}