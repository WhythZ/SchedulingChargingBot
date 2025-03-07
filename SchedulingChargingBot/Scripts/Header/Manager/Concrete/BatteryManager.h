#ifndef _BATTERY_MANAER_H_
#define _BATTERY_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/Concrete/Battery.h"

class BatteryManager :public Manager<BatteryManager>
{
	friend class Manager<BatteryManager>;

private:
	std::vector<Battery*> batteryList;

public:
	void SpawnAt(SDL_Point);                 //以瓦片地图的瓦片索引间接实例化该实体

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

private:
	BatteryManager() = default;
	~BatteryManager();

	void RemoveInvalid();                    //移除非法实例
};

#endif
