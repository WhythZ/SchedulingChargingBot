#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "../Chargeable/Chargeable.h"

struct Strategy
{
	//Robot�����Strategy*��Ա���˴��̳��Ի����ֹѭ������
	virtual void UpdateMovement(Chargeable*) = 0;
};

#endif
