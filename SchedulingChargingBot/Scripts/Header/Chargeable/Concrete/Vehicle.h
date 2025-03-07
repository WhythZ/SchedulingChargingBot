#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"

class Vehicle :public Chargeable
{
public:
	Vehicle();
	~Vehicle() = default;
};

#endif
