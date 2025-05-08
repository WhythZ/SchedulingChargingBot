#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Chargeable.h"
#include "../../Strategy/Strategy.h"

class Robot :public Chargeable
{
private:
	//����ģʽ��ʵ��ִ�е���������ָ�����Ĳ���
	Strategy* strategy = nullptr;

public:
	//Ϊ��ֹѭ����������ֱ��ʹ��Vehicle����ʹ�ø��߳���㼶�Ļ�����Ϊ��Ա����ʹ��ʱǿתΪ(Vehicle*)����
	Chargeable* charged = nullptr;

public:
	Robot();
	~Robot() = default;

	void OnUpdate(double) override;

	void ChangeState(std::string) override;
	void ChangeStrategy(Strategy*);

	bool IsBusy() const override;
};

#endif
