#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"

ChargeableManager::~ChargeableManager()
{
	for (Chargeable* _robot : robotList)
		delete _robot;
	for (Chargeable* _vehicle : vehicleList)
		delete _vehicle;
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
		//�˴�����ʹ��push_back
		robotList.emplace_back((Robot*)_new);
		break;
	case ChargeableType::Vehicle:
		_new = new Vehicle();
		vehicleList.emplace_back((Vehicle*)_new);
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

	for (Robot* _robot : robotList)
		_robot->OnUpdate(_delta);
	for (Vehicle* _vehicle : vehicleList)
		_vehicle->OnUpdate(_delta);
}

void ChargeableManager::OnRender(SDL_Renderer* _renderer)
{
	for (Robot* _robot : robotList)
		_robot->OnRender(_renderer);
	for (Vehicle* _vehicle : vehicleList)
		_vehicle->OnRender(_renderer);
}

void ChargeableManager::TieRobotAndVehicle(Chargeable* _robot, Chargeable* _vehicle)
{
	if (((Robot*)_robot)->charged != nullptr || ((Vehicle*)_vehicle)->charger != nullptr)
		std::cout << "Tie Wrong\n";
	else
	{
		((Robot*)_robot)->charged = _vehicle;
		((Robot*)_robot)->ChangeState("Charger");
		((Vehicle*)_vehicle)->charger = _robot;
		((Vehicle*)_vehicle)->ChangeState("Charged");
	}
}

void ChargeableManager::UntieRobotAndVehicle(Chargeable* _robot, Chargeable* _vehicle)
{
	if (((Robot*)_robot)->charged == nullptr || ((Vehicle*)_vehicle)->charger == nullptr)
		std::cout << "Untie Wrong\n";
	else
	{
		((Robot*)_robot)->charged = nullptr;
		((Robot*)_robot)->ChangeState("Idle");
		((Vehicle*)_vehicle)->charger = nullptr;
		((Vehicle*)_vehicle)->ChangeState("Idle");
	}
}

std::vector<Robot*> ChargeableManager::GetRobotList() const
{
	return robotList;
}

std::vector<Vehicle*> ChargeableManager::GetVehicleList() const
{
	return vehicleList;
}

void ChargeableManager::RemoveInvalid()
{
	#pragma region RemoveInvalidRobot
	auto _beginV = std::remove_if(robotList.begin(), robotList.end(),
		[](const Robot* _robot)
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
		[](const Vehicle* _vehicle)
		{
			if (!_vehicle->IsValid())
			{
				delete _vehicle;
				return true;
			}
			return false;
		});

	//ɾ��������Чʵ������ʱ���б���remove_if�������£�������Ч��ʵ��ָ������б�ĩβ
	vehicleList.erase(_beginR, vehicleList.end());
	#pragma endregion
}