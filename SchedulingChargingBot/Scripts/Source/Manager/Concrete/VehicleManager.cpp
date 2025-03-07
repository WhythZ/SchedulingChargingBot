#include "../../../Header/Manager/Concrete/VehicleManager.h"
#include "../../../Header/Tilemap/Tile.h"

VehicleManager::~VehicleManager()
{
	for (Vehicle* _vehicle : vehicleList)
		delete _vehicle;
}

void VehicleManager::SpawnAt(SDL_Point _point)
{
	Vehicle* _new = new Vehicle();

	//根据传入的瓦片地图索引坐标，计算对应位置瓦片中心点位置
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);

	//推送该实例入列表
	vehicleList.push_back(_new);
}

void VehicleManager::OnUpdate(double _delta)
{
	//移除非法实例
	RemoveInvalid();

	for (Vehicle* _vehicle : vehicleList)
		_vehicle->OnUpdate(_delta);
}

void VehicleManager::OnRender(SDL_Renderer* _renderer)
{
	for (Vehicle* _vehicle : vehicleList)
		_vehicle->OnRender(_renderer);
}

void VehicleManager::RemoveInvalid()
{
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
	auto _begin = std::remove_if(vehicleList.begin(), vehicleList.end(),
		[](const Vehicle* _bullet)
		{
			if (!_bullet->IsValid())
			{
				delete _bullet;
				return true;
			}
			return false;
		});

	//删除所有无效的子弹，此时的列表在remove_if的排列下，所有无效的子弹指针均在列表末尾
	vehicleList.erase(_begin, vehicleList.end());
}