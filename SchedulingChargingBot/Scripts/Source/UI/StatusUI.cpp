#include "../../Header/UI/StatusUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/RobotManager.h"
#include "../../Header/Manager/Concrete/VehicleManager.h"
#include "../../Header/Manager/Concrete/BatteryManager.h"

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
	#pragma endregion

	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;

	#pragma region RobotNumText
	//�õ����ϻ�����������ǿתΪ���κ�ת��Ϊ�ַ���
	static RobotManager* _rm = RobotManager::Instance();
	std::string _robotNumStr = "RobotNum=" + std::to_string((int)_rm->GetRobotList().size());
	SDL_Surface* _robotTextSurface = TTF_RenderText_Blended(_font, _robotNumStr.c_str(), textColor);
	//��ȡת�����ͼƬ�ĳ���
	robotNumTextSize = { _robotTextSurface->w, _robotTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	robotNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _robotTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_robotTextSurface);
	#pragma endregion

	#pragma region VehicleNumText
	static VehicleManager* _vm = VehicleManager::Instance();
	std::string _vehicleNumStr = "VehicleNum=" + std::to_string((int)_vm->GetVehicleList().size());
	SDL_Surface* _vehicleTextSurface = TTF_RenderText_Blended(_font, _vehicleNumStr.c_str(), textColor);
	vehicleNumTextSize = { _vehicleTextSurface->w, _vehicleTextSurface->h };
	vehicleNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _vehicleTextSurface);
	SDL_FreeSurface(_vehicleTextSurface);
	#pragma endregion

	#pragma region BatteryNumText
	static BatteryManager* _bm = BatteryManager::Instance();
	std::string _batteryNumStr = "BatteryNum=" + std::to_string((int)_bm->GetBatteryList().size());
	SDL_Surface* _batteryTextSurface = TTF_RenderText_Blended(_font, _batteryNumStr.c_str(), textColor);
	batteryNumTextSize = { _batteryTextSurface->w, _batteryTextSurface->h };
	batteryNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _batteryTextSurface);
	SDL_FreeSurface(_batteryTextSurface);
	#pragma endregion
}

void StatusUI::OnRender(SDL_Renderer* _renderer)
{
	//����������Ⱦ��ط���
	static UIManager* _ui = UIManager::Instance();

	//���õ����϶�������
	static SDL_Point _positionLeftUp = { 0,0 };

	//��ͼRect
	static SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;

	#pragma region RobotNumText
	//��Ⱦ����Ļ����
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - robotNumTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y;
	robotNumTextSize.x = (int)(robotNumTextSize.x * textZoomRate);
	robotNumTextSize.y = (int)(robotNumTextSize.y * textZoomRate);
	_ui->DrawTexture(_renderer, robotNumTextTexture, _positionLeftUp, robotNumTextSize);
	#pragma endregion

	#pragma region VehicleNumText
	//��Ⱦ����Ļ����
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - vehicleNumTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y + robotNumTextSize.y + rowDistance;
	vehicleNumTextSize.x = (int)(vehicleNumTextSize.x * textZoomRate);
	vehicleNumTextSize.y = (int)(vehicleNumTextSize.y * textZoomRate);
	_ui->DrawTexture(_renderer, vehicleNumTextTexture, _positionLeftUp, vehicleNumTextSize);
	#pragma endregion

	#pragma region BatteryNumText
	//��Ⱦ����Ļ����
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - batteryNumTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y + robotNumTextSize.y + rowDistance + vehicleNumTextSize.y + rowDistance;
	batteryNumTextSize.x = (int)(batteryNumTextSize.x * textZoomRate);
	batteryNumTextSize.y = (int)(batteryNumTextSize.y * textZoomRate);
	_ui->DrawTexture(_renderer, batteryNumTextTexture, _positionLeftUp, batteryNumTextSize);
	#pragma endregion
}