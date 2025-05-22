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
		double vehicleSpeed = 1000;
		double robotSpeed = 1000;
		//�������Ƶ��
		double chargedCooldown = 0.01;
		//�����ŵ�Ƶ��
		double chargerCooldown = 0.01;
	};
	//����ģ��������ϢԤ����
	struct SimulationConfigPrefab
	{
		//�����ģ
		size_t vehicleNumLevelSmall = 10;
		size_t vehicleNumLevelMedium = 20;
		size_t vehicleNumLevelLarge = 30;
		size_t robotNumLevelSmall = 2;
		size_t robotNumLevelMedium = 4;
		size_t robotNumLevelLarge = 6;

		//��������ʱ��������
		size_t vehicleSpawnTimeUpperLevelSmall = 5;
		size_t vehicleSpawnTimeUpperLevelMedium = 4;
		size_t vehicleSpawnTimeUpperLevelLarge = 3;

		//�������뿪ʱ��
		size_t vehicleLeaveTimeSpanLevelSmall = 15;
		size_t vehicleLeaveTimeSpanLevelMedium = 12;
		size_t vehicleLeaveTimeSpanLevelLarge = 8;

		//��ʼ�����������Χ�޶�
		size_t vehicleSpawnElectricityUpper = 40;
		size_t vehicleLeaveElectricityLower = 80;
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
