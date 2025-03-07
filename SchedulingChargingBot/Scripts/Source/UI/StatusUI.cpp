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
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(robotNumTextTexture);
	robotNumTextTexture = nullptr;
	SDL_DestroyTexture(vehicleNumTextTexture);
	vehicleNumTextTexture = nullptr;
	SDL_DestroyTexture(batteryNumTextTexture);
	batteryNumTextTexture = nullptr;
	#pragma endregion

	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::VonwaonBitmap16)->second;

	#pragma region RobotNumText
	//拿到场上机器人数量，强转为整形后转化为字符串
	static RobotManager* _rm = RobotManager::Instance();
	std::string _robotNumStr = "RobotNum=" + std::to_string((int)_rm->GetRobotList().size());
	SDL_Surface* _robotTextSurface = TTF_RenderText_Blended(_font, _robotNumStr.c_str(), textColor);
	//获取转化后的图片的长宽
	robotNumTextSize = { _robotTextSurface->w, _robotTextSurface->h };
	//然后再将其转化为纹理格式
	robotNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _robotTextSurface);
	//然后清理已经无用了的Surface垃圾
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
	//引入纹理渲染相关方法
	static UIManager* _ui = UIManager::Instance();

	//复用的左上顶点坐标
	static SDL_Point _positionLeftUp = { 0,0 };

	//地图Rect
	static SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;

	#pragma region RobotNumText
	//渲染在屏幕中上
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - robotNumTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y;
	robotNumTextSize.x = (int)(robotNumTextSize.x * textZoomRate);
	robotNumTextSize.y = (int)(robotNumTextSize.y * textZoomRate);
	_ui->DrawTexture(_renderer, robotNumTextTexture, _positionLeftUp, robotNumTextSize);
	#pragma endregion

	#pragma region VehicleNumText
	//渲染在屏幕中中
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - vehicleNumTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y + robotNumTextSize.y + rowDistance;
	vehicleNumTextSize.x = (int)(vehicleNumTextSize.x * textZoomRate);
	vehicleNumTextSize.y = (int)(vehicleNumTextSize.y * textZoomRate);
	_ui->DrawTexture(_renderer, vehicleNumTextTexture, _positionLeftUp, vehicleNumTextSize);
	#pragma endregion

	#pragma region BatteryNumText
	//渲染在屏幕中下
	_positionLeftUp.x = _mapRect.x + _mapRect.w / 2 - batteryNumTextSize.x / 2;
	_positionLeftUp.y = _mapRect.y + robotNumTextSize.y + rowDistance + vehicleNumTextSize.y + rowDistance;
	batteryNumTextSize.x = (int)(batteryNumTextSize.x * textZoomRate);
	batteryNumTextSize.y = (int)(batteryNumTextSize.y * textZoomRate);
	_ui->DrawTexture(_renderer, batteryNumTextTexture, _positionLeftUp, batteryNumTextSize);
	#pragma endregion
}