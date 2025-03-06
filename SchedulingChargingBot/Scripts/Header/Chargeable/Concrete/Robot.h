#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Chargeable.h"

class Robot :public Chargeable
{
private:
	double speed = 1;                          //速率大小，1瓦片/单位时间
	Vector2 velocity;                          //速度向量

public:
	Robot();
	~Robot() = default;

	void OnUpdate(double) override;
};

#endif
