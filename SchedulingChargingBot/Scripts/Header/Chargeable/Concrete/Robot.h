#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Chargeable.h"
#include "Vehicle.h"

class Robot :public Chargeable
{
public:
	Chargeable* charged = nullptr;

public:
	Robot();
	~Robot() = default;

	void OnUpdate(double) override;

	void ChangeState(std::string) override;

	bool IsBusy() const override;
};

#endif
