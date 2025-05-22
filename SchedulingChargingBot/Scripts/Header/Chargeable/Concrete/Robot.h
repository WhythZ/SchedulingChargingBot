#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Chargeable.h"
#include "../../Strategy/Strategy.h"

class Robot :public Chargeable
{
public:
	//Ϊ��ֹѭ����������ֱ��ʹ��Vehicle����ʹ�ø��߳���㼶�Ļ�����Ϊ��Ա����ʹ��ʱǿתΪ(Vehicle*)����
	Chargeable* charged = nullptr;
	Chargeable* bestTarget = nullptr;
	double lowestElectricity = 0;

private:
	//����ģʽ��ʵ��ִ�е���������ָ�����Ĳ���
	Strategy* strategy = nullptr;
	Timer movingDrainTimer;

public:
	Robot();
	~Robot() = default;

	void OnUpdate(double) override;

	void ChangeStrategy(Strategy*);
	void ChangeState(std::string) override;

	bool IsBusy() const override;
	bool EnoughElectricity() const;
};

#endif
