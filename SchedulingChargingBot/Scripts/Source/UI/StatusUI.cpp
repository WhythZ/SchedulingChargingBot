#include "../../Header/UI/StatusUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"
#include "../../Header/Manager/Concrete/ScoreManager.h"
#include "../../Header/Manager/Concrete/SpawnManager.h"

void StatusUI::OnUpdate(SDL_Renderer* _renderer)
{
	#pragma region Clear
	//����ÿ֡OnUpdate������������һ���ı������������������һ֡����������
	SDL_DestroyTexture(robotNumTextTexture);
	robotNumTextTexture = nullptr;
	SDL_DestroyTexture(vehicleNumTextTexture);
	vehicleNumTextTexture = nullptr;
	SDL_DestroyTexture(batteryNumTextTexture);
	batteryNumTextTexture = nullptr;
	SDL_DestroyTexture(timeTextTexture);
	timeTextTexture = nullptr;
	SDL_DestroyTexture(hitTextTexture);
	hitTextTexture = nullptr;
	SDL_DestroyTexture(missTextTexture);
	missTextTexture = nullptr;
	#pragma endregion

	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static ScoreManager* _scm = ScoreManager::Instance();
	static SpawnManager* _spm = SpawnManager::Instance();


	#pragma region RobotNumText
	//ת��Ϊ�ַ���
	std::string _robotNumStr = "RNum=" + std::to_string(_cm->GetRobotList().size());
	SDL_Surface* _robotTextSurface = TTF_RenderText_Blended(_font, _robotNumStr.c_str(), textColorNormal);
	//��ȡת�����ͼƬ�ĳ���
	robotNumTextSize = { _robotTextSurface->w, _robotTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	robotNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _robotTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_robotTextSurface);
	#pragma endregion

	#pragma region VehicleNumText
	std::string _vehicleNumStr = "VNum=" + std::to_string(_cm->GetVehicleList().size());
	SDL_Surface* _vehicleTextSurface = TTF_RenderText_Blended(_font, _vehicleNumStr.c_str(), textColorNormal);
	vehicleNumTextSize = { _vehicleTextSurface->w, _vehicleTextSurface->h };
	vehicleNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _vehicleTextSurface);
	SDL_FreeSurface(_vehicleTextSurface);
	#pragma endregion

	#pragma region BatteryNumText
	std::string _batteryNumStr = "BNum=" + std::to_string(_cm->GetBatteryList().size());
	SDL_Surface* _batteryTextSurface = TTF_RenderText_Blended(_font, _batteryNumStr.c_str(), textColorNormal);
	batteryNumTextSize = { _batteryTextSurface->w, _batteryTextSurface->h };
	batteryNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _batteryTextSurface);
	SDL_FreeSurface(_batteryTextSurface);
	#pragma endregion

	#pragma region TimeText
	std::string _timeStr = "Time=" + std::to_string((int)(_scm->GetPassTime()));
	SDL_Surface* _timeTextSurface = TTF_RenderText_Blended(_font, _timeStr.c_str(), textColorNormal);
	timeTextSize = { _timeTextSurface->w, _timeTextSurface->h };
	timeTextTexture = SDL_CreateTextureFromSurface(_renderer, _timeTextSurface);
	SDL_FreeSurface(_timeTextSurface);
	#pragma endregion

	#pragma region HitNumText
	std::string _hitNumStr = "Hit=" + std::to_string((int)(_spm->GetHitVehicleNum()));
	SDL_Surface* _hitNumTextSurface = TTF_RenderText_Blended(_font, _hitNumStr.c_str(), textColorSpecial);
	hitTextSize = { _hitNumTextSurface->w, _hitNumTextSurface->h };
	hitTextTexture = SDL_CreateTextureFromSurface(_renderer, _hitNumTextSurface);
	SDL_FreeSurface(_hitNumTextSurface);
	#pragma endregion

	#pragma region MissNumText
	std::string _missNumStr = "Miss=" + std::to_string((int)(_spm->GetMissVehicleNum()));
	SDL_Surface* _missNumTextSurface = TTF_RenderText_Blended(_font, _missNumStr.c_str(), textColorSpecial);
	missTextSize = { _missNumTextSurface->w, _missNumTextSurface->h };
	missTextTexture = SDL_CreateTextureFromSurface(_renderer, _missNumTextSurface);
	SDL_FreeSurface(_missNumTextSurface);
	#pragma endregion
}

void StatusUI::OnRender(SDL_Renderer* _renderer)
{
	//����������Ⱦ��ط���
	static UIManager* _ui = UIManager::Instance();
	static double _textZoomRate = _ui->textZoomRate;

	//���õ����϶�������
	static SDL_Point _positionLeftUp = { 0,0 };

	//��ͼRect
	static SDL_Rect _mapRect = SceneManager::Instance()->mapRect;

	#pragma region RobotNumText
	//�����ı���С
	robotNumTextSize.x = (int)(robotNumTextSize.x * _textZoomRate);
	robotNumTextSize.y = (int)(robotNumTextSize.y * _textZoomRate);
	//��Ⱦ����Ļ�������
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - robotNumTextSize.x / 2
		- 3 * TILE_SIZE;
	_positionLeftUp.y = _mapRect.y;
	_ui->DrawTexture(_renderer, robotNumTextTexture, _positionLeftUp, robotNumTextSize);
	#pragma endregion

	#pragma region VehicleNumText
	//�����ı���С
	vehicleNumTextSize.x = (int)(vehicleNumTextSize.x * _textZoomRate);
	vehicleNumTextSize.y = (int)(vehicleNumTextSize.y * _textZoomRate);
	//��Ⱦ����Ļ�����м�
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - vehicleNumTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y;
	_ui->DrawTexture(_renderer, vehicleNumTextTexture, _positionLeftUp, vehicleNumTextSize);
	#pragma endregion

	#pragma region BatteryNumText
	//�����ı���С
	batteryNumTextSize.x = (int)(batteryNumTextSize.x * _textZoomRate);
	batteryNumTextSize.y = (int)(batteryNumTextSize.y * _textZoomRate);
	//��Ⱦ����Ļ�����Ҳ�
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - batteryNumTextSize.x / 2
		+ 3 * TILE_SIZE;
	_positionLeftUp.y = _mapRect.y;
	_ui->DrawTexture(_renderer, batteryNumTextTexture, _positionLeftUp, batteryNumTextSize);
	#pragma endregion

	#pragma region TimeText
	//�����ı���С
	timeTextSize.x = (int)(timeTextSize.x * _textZoomRate);
	timeTextSize.y = (int)(timeTextSize.y * _textZoomRate);
	//��Ⱦ�ڵ���ı��·�
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - timeTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y + vehicleNumTextSize.y + rowDistance;
	_ui->DrawTexture(_renderer, timeTextTexture, _positionLeftUp, timeTextSize);
	#pragma endregion

	#pragma region HitNumText
	//�����ı���С
	hitTextSize.x = (int)(hitTextSize.x * _textZoomRate);
	hitTextSize.y = (int)(hitTextSize.y * _textZoomRate);
	//��Ⱦ��ʱ���ı����
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - hitTextSize.x / 2
		- 3 * TILE_SIZE;
	_positionLeftUp.y = _mapRect.y + vehicleNumTextSize.y + rowDistance;
	_ui->DrawTexture(_renderer, hitTextTexture, _positionLeftUp, hitTextSize);
	#pragma endregion

	#pragma region MissNumText
	//�����ı���С
	missTextSize.x = (int)(missTextSize.x * _textZoomRate);
	missTextSize.y = (int)(missTextSize.y * _textZoomRate);
	//��Ⱦ��ʱ���ı��Ҳ�
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - missTextSize.x / 2
		+ 3 * TILE_SIZE;
	_positionLeftUp.y = _mapRect.y + vehicleNumTextSize.y + rowDistance;
	_ui->DrawTexture(_renderer, missTextTexture, _positionLeftUp, missTextSize);
	#pragma endregion
}