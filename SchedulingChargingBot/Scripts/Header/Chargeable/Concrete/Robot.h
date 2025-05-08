#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Chargeable.h"
#include "../../Strategy/Strategy.h"

class Robot :public Chargeable
{
private:
	//策略模式，实际执行的是派生类指针代表的策略
	Strategy* strategy = nullptr;

public:
	//为防止循环依赖，不直接使用Vehicle而是使用更高抽象层级的基类作为成员，在使用时强转为(Vehicle*)即可
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
