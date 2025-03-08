#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Tilemap/Tile.h"

ChargeableManager::~ChargeableManager()
{
	for (Chargeable* _robot : robotList)
		delete _robot;
	for (Chargeable* _vehicle : vehicleList)
		delete _vehicle;
	for (Chargeable* _battery : batteryList)
		delete _battery;
}

void ChargeableManager::SpawnChargeableAt(ChargeableType _type, SDL_Point _point)
{
	//����ָ������ʵ������Ӧ����
	Chargeable* _new = nullptr;
	switch (_type)
	{
	case ChargeableType::None:
		break;
	case ChargeableType::Robot:
		_new = new Robot();
		robotList.push_back(_new);
		break;
	case ChargeableType::Vehicle:
		_new = new Vehicle();
		vehicleList.push_back(_new);
		break;
	case ChargeableType::Battery:
		_new = new Battery();
		batteryList.push_back(_new);
		break;
	default:
		break;
	}
	if (_new == nullptr) return;

	//���ݴ������Ƭ��ͼ�������꣬�����Ӧλ����Ƭ���ĵ�λ��
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);
}

void ChargeableManager::OnUpdate(double _delta)
{
	//�Ƴ��Ƿ�ʵ��
	RemoveInvalid();

	for (Chargeable* _robot : robotList)
		_robot->OnUpdate(_delta);
	for (Chargeable* _vehicle : vehicleList)
		_vehicle->OnUpdate(_delta);
	for (Chargeable* _battery : batteryList)
		_battery->OnUpdate(_delta);
}

void ChargeableManager::OnRender(SDL_Renderer* _renderer)
{
	for (Chargeable* _robot : robotList)
		_robot->OnRender(_renderer);
	for (Chargeable* _vehicle : vehicleList)
		_vehicle->OnRender(_renderer);
	for (Chargeable* _battery : batteryList)
		_battery->OnRender(_renderer);
}

std::vector<Chargeable*> ChargeableManager::GetRobotList() const
{
	return robotList;
}

std::vector<Chargeable*> ChargeableManager::GetVehicleList() const
{
	return vehicleList;
}

std::vector<Chargeable*> ChargeableManager::GetBatteryList() const
{
	return batteryList;
}

void ChargeableManager::RemoveInvalid()
{
	#pragma region RemoveInvalidRobot
	auto _beginV = std::remove_if(robotList.begin(), robotList.end(),
		[](const Chargeable* _robot)
		{
			if (!_robot->IsValid())
			{
				delete _robot;
				return true;
			}
			return false;
		});

	robotList.erase(_beginV, robotList.end());
	#pragma endregion

	#pragma region RemoveInvalidVehicle
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ�����б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
	auto _beginR = std::remove_if(vehicleList.begin(), vehicleList.end(),
		[](const Chargeable* _bullet)
		{
			if (!_bullet->IsValid())
			{
				delete _bullet;
				return true;
			}
			return false;
		});

	//ɾ��������Чʵ������ʱ���б���remove_if�������£�������Ч��ʵ��ָ������б�ĩβ
	vehicleList.erase(_beginR, vehicleList.end());
	#pragma endregion

	#pragma region RemoveInvalidBattery
	auto _beginB = std::remove_if(batteryList.begin(), batteryList.end(),
		[](const Chargeable* _battery)
		{
			if (!_battery->IsValid())
			{
				delete _battery;
				return true;
			}
			return false;
		});

	batteryList.erase(_beginB, batteryList.end());
	#pragma endregion
}
