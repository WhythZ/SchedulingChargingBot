#include "../../../Header/Manager/Concrete/BatteryManager.h"

BatteryManager::~BatteryManager()
{
	for (Battery* _battery : batteryList)
		delete _battery;
}

void BatteryManager::SpawnAt(SDL_Point _point)
{
	Battery* _new = new Battery();

	//���ݴ������Ƭ��ͼ�������꣬�����Ӧλ����Ƭ���ĵ�λ��
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);

	//���͸�ʵ�����б�
	batteryList.push_back(_new);
}

void BatteryManager::OnUpdate(double _delta)
{
	//�Ƴ��Ƿ�ʵ��
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
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ�����б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
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

	//ɾ��������Ч���ӵ�����ʱ���б���remove_if�������£�������Ч���ӵ�ָ������б�ĩβ
	batteryList.erase(_begin, batteryList.end());
}