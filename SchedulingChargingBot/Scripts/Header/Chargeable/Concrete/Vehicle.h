#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"
#include <atomic>

class Vehicle :public Chargeable
{
public:
	SDL_Rect chargedRect = {};            //����������Ч����
	Chargeable* charger = nullptr;
	std::atomic<Chargeable*> isTargeted = nullptr;
	double TargetedDistance;

public:
	Vehicle();
	~Vehicle() = default;

	void OnUpdate(double) override;

	void ChangeState(std::string) override;

	bool IsBusy() const override;
};

#endif
