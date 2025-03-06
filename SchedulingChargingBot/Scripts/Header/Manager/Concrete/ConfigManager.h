#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cJSON.h>
#include <SDL.h>
#include "../Manager.hpp"

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

public:
	BasicConfigPrefab basicPrefab;

public:
	bool LoadConfig(const std::string&);

private:
	ConfigManager() = default;
	~ConfigManager() = default;

	bool ParseBasicConfigPrefab(BasicConfigPrefab&, cJSON*);
};

#endif
