#ifndef _VEHICLE_MANAER_H_
#define _VEHICLE_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/ChargeableType.h"
#include "../../Chargeable/Concrete/Robot.h"
#include "../../Chargeable/Concrete/Vehicle.h"
#include "../../Chargeable/Concrete/Battery.h"
#include "../../Strategy/StrategyType.h"
#include "../../Strategy/Concrete/StrategyA.h"
#include "../../Strategy/Concrete/StrategyB.h"
#include "../../Strategy/Concrete/StrategyC.h"

class ChargeableManager :public Manager<ChargeableManager>
{
	friend class Manager<ChargeableManager>;

private:
	#pragma region ObjectList
	std::vector<Robot*> robotList;
	std::vector<Vehicle*> vehicleList;
	std::vector<Battery*> batteryList;
	#pragma endregion

	#pragma region RobotStrategy
	StrategyA strategyA;                                       //用作Robot的策略A
	StrategyB strategyB;                                       //用作Robot的策略B
	StrategyC strategyC;                                       //用作Robot的策略C
	Strategy* currentStrategy = &strategyA;                    //当前策略类型，初始为A
	StrategyType currentStrategyType = StrategyType::A;        //当前策略类型枚举
	#pragma endregion

public:
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void SpawnChargeableAt(ChargeableType, SDL_Point);         //以瓦片地图的瓦片索引间接实例化某类机器
	void AddExternalChargeable(ChargeableType, Chargeable*);   //手动添加外部创建的Chargeable对象

	void TieRobotAndVehicle(Chargeable*, Chargeable*);
	void UntieRobotAndVehicle(Chargeable*, Chargeable*);       //注意顺序，先Robot后Vehicle
	void ChangeStrategy(StrategyType);                         //更新所有Robot的策略
	void SwitchElectricityRB(Chargeable*,Chargeable*);         //交换电量，先Robot后Battery
	void ClearAll();		                                   //清空场上所有实例

	std::vector<Robot*> GetRobotList() const;                  //获取机器人实例列表
	std::vector<Vehicle*> GetVehicleList() const;              //获取车辆实例列表
	std::vector<Battery*> GetBatteryList() const;              //获取电池实例列表
	StrategyType GetRobotStrategyType() const;                 //获取当前的机器人策略类型枚举
	
private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                                      //移除非法实例
};

#endif
