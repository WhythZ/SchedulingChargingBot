#ifndef _SCORE_MANAGER_H_
#define _SCORE_MANAGER_H_

#include <SDL.h>
#include "../Manager.hpp"
#include "../../../Header/Manager/Concrete/VehicleSpawner.h"

class ScoreManager :public Manager<ScoreManager>
{
	friend class Manager<ScoreManager>;

public:
	double ScoreTimer();            //����һ��doubleֵ��ʾ��ǰʱ��

private:
	ScoreManager() = default;
	~ScoreManager() = default;
};
#endif