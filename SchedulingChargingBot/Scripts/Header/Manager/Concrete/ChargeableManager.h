#ifndef _VEHICLE_MANAER_H_
#define _VEHICLE_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/ChargeableType.h"
#include "../../Chargeable/Concrete/Robot.h"
#include "../../Chargeable/Concrete/Vehicle.h"

class ChargeableManager :public Manager<ChargeableManager>
{
	friend class Manager<ChargeableManager>;

private:
	std::vector<Robot*> robotList;
	std::vector<Vehicle*> vehicleList;

public:
	void SpawnChargeableAt(ChargeableType, SDL_Point);  //以瓦片地图的瓦片索引间接实例化某类机器

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	std::vector<Robot*> GetRobotList() const;           //获取机器人实例列表
	std::vector<Vehicle*> GetVehicleList() const;       //获取车辆实例列表

private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                               //移除非法实例
	void HandleStates();                                //处理充电的双向通知
};

#endif
