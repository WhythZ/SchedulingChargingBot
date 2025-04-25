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

public:
	bool LoadConfig(const std::string&);
	bool LoadMap(const std::string&);

private:
	ConfigManager() = default;
	~ConfigManager() = default;

	bool ParseBasicConfigPrefab(BasicConfigPrefab&, cJSON*);
};

#endif
