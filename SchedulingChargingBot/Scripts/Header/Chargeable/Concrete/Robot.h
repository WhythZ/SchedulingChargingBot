#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Chargeable.h"

class Robot :public Chargeable
{
public:
	Robot();
	~Robot() = default;

	void OnUpdate(double) override;

	void ChangeState(std::string) override;
};

#endif
