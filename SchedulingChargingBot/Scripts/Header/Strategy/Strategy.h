#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "../Chargeable/Chargeable.h"

class Strategy
{
public:
	//Robot�����Strategy*��Ա���˴��̳��Ի����ֹѭ������
	virtual void UpdateMovement(Chargeable*) = 0;
};

#endif
