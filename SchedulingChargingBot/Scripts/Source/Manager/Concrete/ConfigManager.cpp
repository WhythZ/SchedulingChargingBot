#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../../../Header/Manager/Concrete/SceneManager.h"

bool ConfigManager::LoadConfig(const std::string& _path)
{
	#pragma region GetJsonRoot
	//从传入路径读取文件，并判断是否读取失败
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//将文件内容一次性读取出来，然后关闭文件
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();

	//将_strStream的内容转化为C风格字符数组后解析并存储（cJSON_Parse方法是开辟到堆区的，故后续要对_jsonRoot进行销毁），检测是否解析成功
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;

	//类型检测
	if (_jsonRoot->type != cJSON_Object) return false;
	#pragma endregion

	//解析基本配置
	if (!ParseBasicConfigPrefab(basicPrefab, cJSON_GetObjectItem(_jsonRoot, "basic"))) return false;

	//释放内存并返回
	cJSON_Delete(_jsonRoot);
	return true;
}

bool ConfigManager::LoadMap(const std::string& _csvPath)
{
	return SceneManager::Instance()->map.Load(_csvPath);
}

bool ConfigManager::ParseBasicConfigPrefab(BasicConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//检测传入类型
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