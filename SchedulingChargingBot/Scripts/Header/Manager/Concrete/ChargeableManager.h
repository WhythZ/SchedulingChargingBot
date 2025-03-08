#ifndef _VEHICLE_MANAER_H_
#define _VEHICLE_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/ChargeableType.h"
#include "../../Chargeable/Concrete/Robot.h"
#include "../../Chargeable/Concrete/Vehicle.h"
#include "../../Chargeable/Concrete/Battery.h"

class ChargeableManager :public Manager<ChargeableManager>
{
	friend class Manager<ChargeableManager>;

private:
	std::vector<Chargeable*> robotList;
	std::vector<Chargeable*> vehicleList;
	std::vector<Chargeable*> batteryList;

public:
	void SpawnChargeableAt(ChargeableType, SDL_Point);  //以瓦片地图的瓦片索引间接实例化某类机器

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	std::vector<Chargeable*> GetRobotList() const;      //获取机器人实例列表
	std::vector<Chargeable*> GetVehicleList() const;    //获取车辆实例列表
	std::vector<Chargeable*> GetBatteryList() const;    //获取电池实例列表

private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                               //移除非法实例
};

#endif
