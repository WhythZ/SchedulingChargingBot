#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"
#include "Robot.h"

class Vehicle :public Chargeable
{
public:
	SDL_Rect chargedRect = {};            //����������Ч����

public:
	Vehicle();
	~Vehicle() = default;

	void OnUpdate(double) override;

	void ChangeState(std::string) override;
};

#endif
