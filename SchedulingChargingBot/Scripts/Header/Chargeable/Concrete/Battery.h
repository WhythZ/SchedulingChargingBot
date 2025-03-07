#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "../Chargeable.h"

class Battery :public Chargeable
{
public:
	Battery();
	~Battery() = default;

	void OnUpdate(double) override;
};

#endif
