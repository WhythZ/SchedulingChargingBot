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
	void SpawnChargeableAt(ChargeableType, SDL_Point);  //����Ƭ��ͼ����Ƭ�������ʵ����ĳ�����

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	std::vector<Robot*> GetRobotList() const;           //��ȡ������ʵ���б�
	std::vector<Vehicle*> GetVehicleList() const;       //��ȡ����ʵ���б�

private:
	ChargeableManager() = default;
	~ChargeableManager();

	void RemoveInvalid();                               //�Ƴ��Ƿ�ʵ��
	void HandleStates();                                //�������˫��֪ͨ
};

#endif
