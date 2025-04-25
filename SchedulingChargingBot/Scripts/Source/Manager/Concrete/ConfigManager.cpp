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