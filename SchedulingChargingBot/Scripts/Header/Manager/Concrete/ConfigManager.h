#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <string>
#include <cJSON.h>
#include <SDL.h>
#include "../Manager.hpp"
#include "../../Tilemap/Map.h"

//���ڴ�����Ϸ�����ļ�����
class ConfigManager :public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;

public:
	//����������ϢԤ����
	struct BasicConfigPrefab
	{
		//���ڸ߶�����
		int windowWidth = 1024;
		int windowHeight = 1024;
		//���ڱ���
		std::string windowTitle = "SchedulingChargingBot";
	};
	//Chargeable������ϢԤ����
	struct ChargeableConfigPrefab
	{
		//�ƶ��ٶ�
		double speed;
		//�������Ƶ��
		double chargedCooldown;
		//�����ŵ�Ƶ��
		double chargerCooldown;
	};
	//����ģ��������ϢԤ����
	struct SimulationConfigPrefab
	{
		//�����ģ
		size_t vehicleNumLevelSmall;
		size_t vehicleNumLevelMedium;
		size_t vehicleNumLevelLarge;
		size_t robotNumLevelSmall;
		size_t robotNumLevelMedium;
		size_t robotNumLevelLarge;

		//��������ʱ�������ޡ��뿪ʱ��
		size_t vehicleSpawnTimeUpper;
		size_t vehicleLeaveTimeSpan;

		//��ʼ�����������Χ�޶�
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
