#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Chargeable/Concrete/Vehicle.h"
#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Chargeable/Concrete/Battery.h"

ChargeableManager::~ChargeableManager()
{
	for (Chargeable* _robot : robotList)
		delete _robot;
	for (Chargeable* _vehicle : vehicleList)
		delete _vehicle;
}

void ChargeableManager::SpawnChargeableAt(ChargeableType _type, SDL_Point _point)
{
	//依据指定类型实例化对应子类
	Chargeable* _new = nullptr;
	switch (_type)
	{
	case ChargeableType::None:
		break;
	case ChargeableType::Robot:
		_new = new Robot();
		//采取当前策略类型
		((Robot*)_new)->ChangeStrategy(currentStrategy);
		//强转为Robot*再塞入
		robotList.emplace_back((Robot*)_new);
		break;
	case ChargeableType::Vehicle:
		_new = new Vehicle();
		vehicleList.emplace_back((Vehicle*)_new);
		break;
	case ChargeableType::Battery:
		_new = new Battery();
		batteryList.emplace_back((Battery*)_new);
		break;
	default:
		break;
	}
	if (_new == nullptr) return;

	//根据传入的瓦片地图索引坐标，计算对应位置瓦片中心点位置
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;      // TILE_SIZE==64
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);
}

void ChargeableManager::OnUpdate(double _delta)
{

	//移除非法实例
	RemoveInvalid();

	//更新所有实例
	for (Vehicle* _v : vehicleList)
	{
		if (_v->isOnline)
			_v->OnUpdate(_delta);
	}
	for (Robot* _robot : robotList)
		_robot->OnUpdate(_delta);
	for (Battery* _battery : batteryList)
		_battery->OnUpdate(_delta);
}

void ChargeableManager::OnRender(SDL_Renderer* _renderer)
{
	for (Robot* _robot : robotList)
		_robot->OnRender(_renderer);
	for (Vehicle* _vehicle : vehicleList)
		_vehicle->OnRender(_renderer);
	for (Battery* _battery : batteryList)
		_battery->OnRender(_renderer);
}

void ChargeableManager::TieRobotAndVehicle(Chargeable* _robot, Chargeable* _vehicle)
{
	if (_robot != nullptr && _vehicle != nullptr)
	{
		((Robot*)_robot)->charged = _vehicle;
		((Robot*)_robot)->ChangeState("Charger");
		((Robot*)_robot)->SetVelocity({ 0, 0 });
	
		((Vehicle*)_vehicle)->charger = _robot;
		((Vehicle*)_vehicle)->ChangeState("Charged");
	}
}

void ChargeableManager::UntieRobotAndVehicle(Chargeable* _robot, Chargeable* _vehicle)
{	
	if (_robot != nullptr && _vehicle != nullptr)
	{
		((Robot*)_robot)->ChangeState("Idle");
		((Robot*)_robot)->charged = nullptr;
		((Robot*)_robot)->bestTarget = nullptr;

		((Vehicle*)_vehicle)->ChangeState("Idle");
		((Vehicle*)_vehicle)->charger = nullptr;
		((Vehicle*)_vehicle)->isTargeted = nullptr;

		//若满电，则在解绑时清除
		if (!_vehicle->NeedElectricity())
		{
			_vehicle->Invalidate();
			std::cout << "Veicle Leave\n";
		}
	}
}

void ChargeableManager::ChangeStrategy(StrategyType _type)
{
	//更新策略类型
	switch (_type)
	{
	case StrategyType::A:
		for (Robot* _robot : robotList)
			_robot->ChangeStrategy(&strategyA);
		currentStrategy = &strategyA;
		currentStrategyType = StrategyType::A;
		break;
	case StrategyType::B:
		for (Robot* _robot : robotList)
			_robot->ChangeStrategy(&strategyB);
		currentStrategy = &strategyB;
		currentStrategyType = StrategyType::B;
		break;
	case StrategyType::C:
		for (Robot* _robot : robotList)
			_robot->ChangeStrategy(&strategyC);
		currentStrategy = &strategyC;
		currentStrategyType = StrategyType::C;
		break;
	default:
		break;
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

std::vector<Battery*> ChargeableManager::GetBatteryList() const
{
	return batteryList;
}

StrategyType ChargeableManager::GetRobotStrategyType() const
{
	return currentStrategyType;
}

void ChargeableManager::RemoveInvalid()
{
	#pragma region RemoveInvalidRobot
	auto _beginR = std::remove_if(robotList.begin(), robotList.end(),
		[](const Robot* _robot)
		{
			if (!_robot->IsValid())
			{
				delete _robot;
				return true;
			}
			return false;
		});

	robotList.erase(_beginR, robotList.end());
	#pragma endregion

	#pragma region RemoveInvalidVehicle
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
	auto _beginV = std::remove_if(vehicleList.begin(), vehicleList.end(),
		[](const Vehicle* _vehicle)
		{
			if (!_vehicle->IsValid())
			{
				delete _vehicle;
				return true;
			}
			return false;
		});

	//删除所有无效实例，此时的列表在remove_if的排列下，所有无效的实例指针均在列表末尾
	vehicleList.erase(_beginV, vehicleList.end());
	#pragma endregion

	#pragma region RemoveInvalidBattery
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
	auto _beginB = std::remove_if(batteryList.begin(), batteryList.end(),
		[](const Battery* _battery)
		{
			if (!_battery->IsValid())
			{
				delete _battery;
				return true;
			}
			return false;
		});

	//删除所有无效实例，此时的列表在remove_if的排列下，所有无效的实例指针均在列表末尾
	batteryList.erase(_beginB, batteryList.end());
	#pragma endregion
}

void ChargeableManager::AddChargeable(Chargeable* c)
{
	if (Vehicle* v = dynamic_cast<Vehicle*>(c)) {
		vehicleList.emplace_back(v);
	}
	else if (Robot* r = dynamic_cast<Robot*>(c)) {
		robotList.emplace_back(r);
	}
	else if (Battery* b = dynamic_cast<Battery*>(c)) {
		batteryList.emplace_back(b);
	}
}