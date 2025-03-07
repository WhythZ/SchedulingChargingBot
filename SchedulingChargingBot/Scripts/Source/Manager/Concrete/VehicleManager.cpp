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

	//���ݴ������Ƭ��ͼ�������꣬�����Ӧλ����Ƭ���ĵ�λ��
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);

	//���͸�ʵ�����б�
	vehicleList.push_back(_new);
}

void VehicleManager::OnUpdate(double _delta)
{
	//�Ƴ��Ƿ�ʵ��
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
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ�����б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
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

	//ɾ��������Ч���ӵ�����ʱ���б���remove_if�������£�������Ч���ӵ�ָ������б�ĩβ
	vehicleList.erase(_begin, vehicleList.end());
}