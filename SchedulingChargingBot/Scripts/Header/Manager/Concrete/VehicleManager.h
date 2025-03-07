#ifndef _VEHICLE_MANAER_H_
#define _VEHICLE_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/Concrete/Vehicle.h"

class VehicleManager :public Manager<VehicleManager>
{
	friend class Manager<VehicleManager>;

private:
	std::vector<Vehicle*> vehicleList;

public:
	void SpawnAt(SDL_Point);                 //以瓦片地图的瓦片索引间接实例化该实体

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

private:
	VehicleManager() = default;
	~VehicleManager();

	void RemoveInvalid();                    //移除非法实例
};

#endif
