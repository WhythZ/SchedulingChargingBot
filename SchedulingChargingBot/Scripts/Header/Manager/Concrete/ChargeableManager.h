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
	StrategyA strategyA;                                 //����Robot�Ĳ���A
	StrategyB strategyB;                                 //����Robot�Ĳ���B
	StrategyC strategyC;                                 //����Robot�Ĳ���C
	Strategy* currentStrategy = &strategyA;              //��ǰ�������ͣ���ʼΪA
	StrategyType currentStrategyType = StrategyType::A;  //��ǰ��������ö��
	#pragma endregion

public:
	void SpawnChargeableAt(ChargeableType, SDL_Point);   //����Ƭ��ͼ����Ƭ�������ʵ����ĳ�����

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void TieRobotAndVehicle(Chargeable*, Chargeable*);
	void UntieRobotAndVehicle(Chargeable*, Chargeable*); //ע��˳�򣬵�һ���������ˣ��ڶ������ؾ�
	void ChangeStrategy(StrategyType);                   //��������Robot�Ĳ���

	std::vector<Robot*> GetRobotList() const;            //��ȡ������ʵ���б�
	std::vector<Vehicle*> GetVehicleList() const;        //��ȡ����ʵ���б�
	std::vector<Battery*> GetBatteryList() const;        //��ȡ���ʵ���б�
	StrategyType GetRobotStrategyType() const;           //��ȡ��ǰ�Ļ����˲�������ö��
	void AddChargeable(Chargeable* c);

private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                                //�Ƴ��Ƿ�ʵ��
};

#endif
