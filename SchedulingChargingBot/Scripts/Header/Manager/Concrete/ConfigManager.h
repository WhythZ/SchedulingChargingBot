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
	//Chargeable配置信息预制体
	struct ChargeableConfigPrefab
	{
		//移动速度
		double speed;
		//被动充电频率
		double chargedCooldown;
		//主动放电频率
		double chargerCooldown;
	};
	//问题模拟配置信息预制体
	struct SimulationConfigPrefab
	{
		//问题规模
		size_t vehicleNumLevelSmall;
		size_t vehicleNumLevelMedium;
		size_t vehicleNumLevelLarge;
		size_t robotNumLevelSmall;
		size_t robotNumLevelMedium;
		size_t robotNumLevelLarge;

		//车辆生成时间间隔上限、离开时间
		size_t vehicleSpawnTimeUpper;
		size_t vehicleLeaveTimeSpan;

		//初始及需求电量范围限定
		size_t vehicleSpawnElectricityUpper;
		size_t vehicleLeaveElectricityLower;
	};

public:
	BasicConfigPrefab basicPrefab;
	ChargeableConfigPrefab chargeablePrefab;
	SimulationConfigPrefab simulationPrefab;

public:
	bool LoadConfig(const std::string&);
	bool LoadMap(const std::string&);

private:
	ConfigManager() = default;
	~ConfigManager() = default;

	bool ParseBasicConfigPrefab(BasicConfigPrefab&, cJSON*);
	bool ParseChargeableConfigPrefab(ChargeableConfigPrefab&, cJSON*);
	bool ParseSimulationConfigPrefab(SimulationConfigPrefab&, cJSON*);
};

#endif
