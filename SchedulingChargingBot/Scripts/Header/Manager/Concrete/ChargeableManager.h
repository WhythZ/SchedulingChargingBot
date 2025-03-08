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
	void SpawnChargeableAt(ChargeableType, SDL_Point);  //����Ƭ��ͼ����Ƭ�������ʵ����ĳ�����

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	std::vector<Chargeable*> GetRobotList() const;      //��ȡ������ʵ���б�
	std::vector<Chargeable*> GetVehicleList() const;    //��ȡ����ʵ���б�
	std::vector<Chargeable*> GetBatteryList() const;    //��ȡ���ʵ���б�

private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                               //�Ƴ��Ƿ�ʵ��
};

#endif
