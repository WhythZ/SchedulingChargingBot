#ifndef _VEHICLE_MANAER_H_
#define _VEHICLE_MANAER_H_

#include "../Manager.hpp"

class VehicleManager :public Manager<VehicleManager>
{
	friend class Manager<VehicleManager>;
private:
	VehicleManager() = default;
	~VehicleManager() = default;
};

#endif
