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
	StrategyA strategyA;                                 //�洢����Robot��ĳ��ʵ�ʲ���
	StrategyB strategyB;
	Strategy* currentStrategy = &strategyA;              //��ǰ�������ͣ�Ĭ��ΪA
	#pragma endregion

public:
	void SpawnChargeableAt(ChargeableType, SDL_Point);   //����Ƭ��ͼ����Ƭ�������ʵ����ĳ�����

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void TieRobotAndVehicle(Chargeable*, Chargeable*);
	void UntieRobotAndVehicle(Chargeable*, Chargeable*);

	std::vector<Robot*> GetRobotList() const;            //��ȡ������ʵ���б�
	std::vector<Vehicle*> GetVehicleList() const;        //��ȡ����ʵ���б�

private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                                //�Ƴ��Ƿ�ʵ��
	void ChangeStrategy(StrategyType);                   //��������Robot�Ĳ���
};

#endif
