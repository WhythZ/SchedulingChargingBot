#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "../Chargeable.h"
#include <atomic>

class Battery :public Chargeable
{
private:
	Timer movingDrainTimer;

public:
	SDL_Rect chargedRect = {};                //触发充电的有效区域
	std::atomic<bool> isTargeted;

public:
	Battery();
	~Battery() = default;

	void OnUpdate(double) override;

	void ChangeState(std::string) override;

	bool IsBusy() const override;
};

#endif