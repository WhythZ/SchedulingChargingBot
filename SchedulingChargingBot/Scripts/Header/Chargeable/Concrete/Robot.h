#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Chargeable.h"

class Robot :public Chargeable
{
public:
	bool isCursorDragging = false;             //�Ƿ��ڱ������ק��״̬

private:
	double speed = 1;                          //���ʴ�С��1��Ƭ/��λʱ��
	Vector2 velocity;                          //�ٶ�����

public:
	Robot();
	~Robot() = default;
	void SetVelocity(Vector2);                 //�����ٶ�

	void OnUpdate(double) override;
};

#endif
