#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../../../Header/Manager/Concrete/SceneManager.h"

bool ConfigManager::LoadConfig(const std::string& _path)
{
	#pragma region GetJsonRoot
	//�Ӵ���·����ȡ�ļ������ж��Ƿ��ȡʧ��
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//���ļ�����һ���Զ�ȡ������Ȼ��ر��ļ�
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();

	//��_strStream������ת��ΪC����ַ������������洢��cJSON_Parse�����ǿ��ٵ������ģ��ʺ���Ҫ��_jsonRoot�������٣�������Ƿ�����ɹ�
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;

	//���ͼ��
	if (_jsonRoot->type != cJSON_Object) return false;
	#pragma endregion

	//������������
	if (!ParseBasicConfigPrefab(basicPrefab, cJSON_GetObjectItem(_jsonRoot, "basic"))) return false;
	if (!ParseChargeableConfigPrefab(chargeablePrefab, cJSON_GetObjectItem(_jsonRoot, "chargeable"))) return false;
	if (!ParseSimulationConfigPrefab(simulationPrefab, cJSON_GetObjectItem(_jsonRoot, "simulation"))) return false;

	//�ͷ��ڴ沢����
	cJSON_Delete(_jsonRoot);
	return true;
}

bool ConfigManager::LoadMap(const std::string& _csvPath)
{
	return SceneManager::Instance()->map.Load(_csvPath);
}

bool ConfigManager::ParseBasicConfigPrefab(BasicConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//��⴫������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonWindowWidth = cJSON_GetObjectItem(_jsonRoot, "window_width");
	cJSON* _jsonWindowHeight = cJSON_GetObjectItem(_jsonRoot, "window_height");
	cJSON* _jsonWindowTitle = cJSON_GetObjectItem(_jsonRoot, "window_title");

	if (!_jsonWindowWidth || !_jsonWindowHeight || !_jsonWindowTitle
		|| _jsonWindowWidth->type != cJSON_Number
		|| _jsonWindowHeight->type != cJSON_Number
		|| _jsonWindowTitle->type != cJSON_String)
	{
		return false;
	}

	_prefab.windowWidth = _jsonWindowWidth->valueint;
	_prefab.windowHeight = _jsonWindowHeight->valueint;
	_prefab.windowTitle = _jsonWindowTitle->valuestring;

	return true;
}

bool ConfigManager::ParseChargeableConfigPrefab(ChargeableConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//��⴫������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonSpeed = cJSON_GetObjectItem(_jsonRoot, "speed");
	cJSON* _jsonChargedCd = cJSON_GetObjectItem(_jsonRoot, "charged_cooldown");
	cJSON* _jsonChargerCd = cJSON_GetObjectItem(_jsonRoot, "charger_cooldown");

	if (!_jsonSpeed || !_jsonChargedCd || !_jsonChargerCd
		|| _jsonSpeed->type != cJSON_Number
		|| _jsonChargedCd->type != cJSON_Number
		|| _jsonChargerCd->type != cJSON_Number)
	{
		return false;
	}

	_prefab.speed = _jsonSpeed->valuedouble;
	_prefab.chargedCooldown = _jsonChargedCd->valuedouble;
	_prefab.chargerCooldown = _jsonChargerCd->valuedouble;

	return true;
}

bool ConfigManager::ParseSimulationConfigPrefab(SimulationConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//��⴫������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonVNumSmall = cJSON_GetObjectItem(_jsonRoot, "v_num_small");
	cJSON* _jsonVNumMedium = cJSON_GetObjectItem(_jsonRoot, "v_num_medium");
	cJSON* _jsonVNumLarge = cJSON_GetObjectItem(_jsonRoot, "v_num_large");
	cJSON* _jsonRNumSmall = cJSON_GetObjectItem(_jsonRoot, "r_num_small");
	cJSON* _jsonRNumMedium = cJSON_GetObjectItem(_jsonRoot, "r_num_medium");
	cJSON* _jsonRNumLarge = cJSON_GetObjectItem(_jsonRoot, "r_num_large");
	cJSON* _jsonVSpawnTimeUpper = cJSON_GetObjectItem(_jsonRoot, "v_spawn_time_upper");
	cJSON* _jsonVLeaveTimeSpan = cJSON_GetObjectItem(_jsonRoot, "v_leave_time_span");
	cJSON* _jsonVSpawnElecUpper = cJSON_GetObjectItem(_jsonRoot, "v_spawn_elec_upper");
	cJSON* _jsonVLeaveElecLower = cJSON_GetObjectItem(_jsonRoot, "v_leave_elec_lower");

	if (!_jsonVNumSmall || !_jsonVNumMedium || !_jsonVNumLarge
		|| !_jsonRNumSmall || !_jsonRNumMedium || !_jsonRNumLarge
		|| !_jsonVSpawnTimeUpper || !_jsonVLeaveTimeSpan
		|| !_jsonVSpawnElecUpper || !_jsonVLeaveElecLower
		|| _jsonVNumSmall->type != cJSON_Number
		|| _jsonVNumMedium->type != cJSON_Number
		|| _jsonVNumLarge->type != cJSON_Number
		|| _jsonRNumSmall->type != cJSON_Number
		|| _jsonRNumMedium->type != cJSON_Number
		|| _jsonRNumLarge->type != cJSON_Number
		|| !_jsonVSpawnTimeUpper || !_jsonVLeaveTimeSpan
		|| !_jsonVSpawnElecUpper || !_jsonVLeaveElecLower)
	{
		return false;
	}

	_prefab.vehicleNumLevelSmall = (size_t)(_jsonVNumSmall->valueint);
	_prefab.vehicleNumLevelMedium = (size_t)(_jsonVNumMedium->valueint);
	_prefab.vehicleNumLevelLarge = (size_t)(_jsonVNumLarge->valueint);
	_prefab.robotNumLevelSmall = (size_t)(_jsonRNumSmall->valueint);
	_prefab.robotNumLevelMedium = (size_t)(_jsonRNumMedium->valueint);
	_prefab.robotNumLevelLarge = (size_t)(_jsonRNumLarge->valueint);
	_prefab.vehicleSpawnTimeUpper = (size_t)(_jsonVSpawnTimeUpper->valueint);
	_prefab.vehicleLeaveTimeSpan = (size_t)(_jsonVLeaveTimeSpan->valueint);
	_prefab.vehicleSpawnElectricityUpper = (size_t)(_jsonVSpawnElecUpper->valueint);
	_prefab.vehicleLeaveElectricityLower = (size_t)(_jsonVLeaveElecLower->valueint);

	return true;
}