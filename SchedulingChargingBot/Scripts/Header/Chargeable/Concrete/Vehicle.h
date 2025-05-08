#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"

class Vehicle :public Chargeable
{
public:
	SDL_Rect chargedRect = {};            //触发充电的有效区域
	Chargeable* charger = nullptr;

public:
	Vehicle();
	~Vehicle() = default;

	void OnUpdate(double) override;

	void ChangeState(std::string) override;

	bool IsBusy() const override;
};

#endif
