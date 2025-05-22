#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "../Chargeable/Chargeable.h"

class Strategy
{
public:
	//Robot类包含Strategy*成员，此处继承自基类防止循环引用
	virtual void UpdateMovement(Chargeable*) = 0;
};

#endif
