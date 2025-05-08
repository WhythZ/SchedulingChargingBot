#ifndef _VEHICLE_MANAER_H_
#define _VEHICLE_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/ChargeableType.h"
#include "../../Chargeable/Concrete/Robot.h"
#include "../../Chargeable/Concrete/Vehicle.h"
#include "../../Strategy/StrategyType.h"
#include "../../Strategy/Concrete/StrategyA.h"
#include "../../Strategy/Concrete/StrategyB.h"

class ChargeableManager :public Manager<ChargeableManager>
{
	friend class Manager<ChargeableManager>;

private:
	#pragma region Objects
	std::vector<Robot*> robotList;
	std::vector<Vehicle*> vehicleList;
	#pragma endregion

	#pragma region Strategy
	StrategyA strategyA;                                 //存储用作Robot的某种实际策略
	StrategyB strategyB;
	Strategy* currentStrategy = &strategyA;              //当前策略类型，默认为A
	#pragma endregion

public:
	void SpawnChargeableAt(ChargeableType, SDL_Point);   //以瓦片地图的瓦片索引间接实例化某类机器

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void TieRobotAndVehicle(Chargeable*, Chargeable*);
	void UntieRobotAndVehicle(Chargeable*, Chargeable*);

	std::vector<Robot*> GetRobotList() const;            //获取机器人实例列表
	std::vector<Vehicle*> GetVehicleList() const;        //获取车辆实例列表

private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                                //移除非法实例
	void ChangeStrategy(StrategyType);                   //更新所有Robot的策略
};

#endif
