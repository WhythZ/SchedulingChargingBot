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
	//��ͼRect
	SDL_Rect _mapRect = SceneManager::Instance()->mapRect;

	//���԰�ť������Ļ�·������
	strategyButtonRect =
	{
		_mapRect.w / 2 - strategyButtonSize.x / 2 - 3 * TILE_SIZE,
		_mapRect.h - strategyButtonSize.y,
		strategyButtonSize.x,
		strategyButtonSize.y
	};

	//��ģ�ȼ����ڲ��԰�ť�Ҳ�
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
		//������°�ť���л����Ի��ģ
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			#pragma region Strategy
			//�����괦�ڲ��԰�ť��Rect��
			if (SDL_PointInRect(&_cursorPosition, &strategyButtonRect))
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
			#pragma endregion
			
			#pragma region Level
			//�����괦�ڲ��԰�ť��Rect��
			if (SDL_PointInRect(&_cursorPosition, &levelButtonRect))
			{
				//�л�����һ����ģ��ѭ������
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
	//����ÿ֡OnUpdate������������һ���ı������������������һ֡����������
	SDL_DestroyTexture(strategyTypeTextTexture);
	strategyTypeTextTexture = nullptr;
	SDL_DestroyTexture(levelTypeTextTexture);
	levelTypeTextTexture = nullptr;
	#pragma endregion

	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static SpawnManager* _sm = SpawnManager::Instance();

	#pragma region StrategyText
	//ת��Ϊ�ַ���
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
	//��ȡת�����ͼƬ�ĳ���
	strategyTypeTextSize = { _strategyTypeTextSurface->w, _strategyTypeTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	strategyTypeTextTexture = SDL_CreateTextureFromSurface(_renderer, _strategyTypeTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_strategyTypeTextSurface);
	#pragma endregion

	#pragma region LevelText
	//ת��Ϊ�ַ���
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
	//��ȡת�����ͼƬ�ĳ���
	levelTypeTextSize = { _levelTypeTextSurface->w, _levelTypeTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	levelTypeTextTexture = SDL_CreateTextureFromSurface(_renderer, _levelTypeTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_levelTypeTextSurface);
	#pragma endregion
}

void ButtonUI::OnRender(SDL_Renderer* _renderer)
{
	//����������Ⱦ��ط���
	static UIManager* _ui = UIManager::Instance();
	static double _textZoomRate = _ui->textZoomRate;

	//���õ����϶�������
	static SDL_Point _positionLeftUp = { 0,0 };

	#pragma region Button
	//���ư�ť����
	_ui->DrawBox(_renderer, strategyButtonRect, buttonColor);
	_ui->DrawBox(_renderer, levelButtonRect, buttonColor);
	#pragma endregion

	#pragma region Text
	//�����ı���С
	strategyTypeTextSize.x = (int)(strategyTypeTextSize.x * _textZoomRate);
	strategyTypeTextSize.y = (int)(strategyTypeTextSize.y * _textZoomRate);
	//��Ⱦ�ڰ�ť��������
	_positionLeftUp.x = strategyButtonRect.x + strategyButtonRect.w / 2 - strategyTypeTextSize.x / 2;
	_positionLeftUp.y = strategyButtonRect.y + strategyButtonRect.h / 2 - strategyTypeTextSize.y / 2;
	_ui->DrawTexture(_renderer, strategyTypeTextTexture, _positionLeftUp, strategyTypeTextSize);

	//�����ı���С
	levelTypeTextSize.x = (int)(levelTypeTextSize.x * _textZoomRate);
	levelTypeTextSize.y = (int)(levelTypeTextSize.y * _textZoomRate);
	//��Ⱦ�ڰ�ť��������
	_positionLeftUp.x = levelButtonRect.x + levelButtonRect.w / 2 - levelTypeTextSize.x / 2;
	_positionLeftUp.y = levelButtonRect.y + levelButtonRect.h / 2 - levelTypeTextSize.y / 2;
	_ui->DrawTexture(_renderer, levelTypeTextTexture, _positionLeftUp, levelTypeTextSize);
	#pragma endregion
}