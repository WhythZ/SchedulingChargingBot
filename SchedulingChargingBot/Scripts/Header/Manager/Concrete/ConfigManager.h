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

public:
	BasicConfigPrefab basicPrefab;

	#pragma region Map
	//�洢�����ص���Ƭ��ͼ
	Map map;
	//���ڴ�Ŀ�괰�����и��һ��������������mapTexture�Ļ�������
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
