#include "../../../Header/Manager/Concrete/BatteryManager.h"

BatteryManager::~BatteryManager()
{
	for (Battery* _battery : batteryList)
		delete _battery;
}

void BatteryManager::SpawnAt(SDL_Point _point)
{
	Battery* _new = new Battery();

	//根据传入的瓦片地图索引坐标，计算对应位置瓦片中心点位置
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);

	//推送该实例入列表
	batteryList.push_back(_new);
}

void BatteryManager::OnUpdate(double _delta)
{
	//移除非法实例
	RemoveInvalid();

	for (Battery* _battery : batteryList)
		_battery->OnUpdate(_delta);
}

void BatteryManager::OnRender(SDL_Renderer* _renderer)
{
	for (Battery* _battery : batteryList)
		_battery->OnRender(_renderer);
}

void BatteryManager::RemoveInvalid()
{
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
	auto _begin = std::remove_if(batteryList.begin(), batteryList.end(),
		[](const Battery* _battery)
		{
			if (!_battery->IsValid())
			{
				delete _battery;
				return true;
			}
			return false;
		});

	//删除所有无效的子弹，此时的列表在remove_if的排列下，所有无效的子弹指针均在列表末尾
	batteryList.erase(_begin, batteryList.end());
}