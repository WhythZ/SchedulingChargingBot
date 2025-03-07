#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <string>
#include <cJSON.h>
#include <SDL.h>
#include "../Manager.hpp"
#include "../../Tilemap/Map.h"

//用于处理游戏配置文件数据
class ConfigManager :public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;

public:
	//基础配置信息预制体
	struct BasicConfigPrefab
	{
		//窗口高度与宽度
		int windowWidth = 1024;
		int windowHeight = 1024;
		//窗口标题
		std::string windowTitle = "SchedulingChargingBot";
	};

public:
	BasicConfigPrefab basicPrefab;

	#pragma region Map
	//存储被加载的瓦片地图
	Map map;
	//用于从目标窗口中切割出一块区域，用于塞入mapTexture的画面内容
	SDL_Rect mapRect = { 0 };
	#pragma endregion


public:
	bool LoadConfig(const std::string&);
	bool LoadMap(const std::string&);

private:
	ConfigManager() = default;
	~ConfigManager() = default;

	bool ParseBasicConfigPrefab(BasicConfigPrefab&, cJSON*);
};

#endif
