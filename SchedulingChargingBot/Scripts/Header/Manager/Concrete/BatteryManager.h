#ifndef _BATTERY_MANAER_H_
#define _BATTERY_MANAER_H_

#include "../Manager.hpp"

class BatteryManager :public Manager<BatteryManager>
{
	friend class Manager<BatteryManager>;
private:
	BatteryManager() = default;
	~BatteryManager() = default;
};

#endif
