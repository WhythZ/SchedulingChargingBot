#ifndef _SCORE_MANAGER_H_
#define _SCORE_MANAGER_H_

#include <SDL.h>
#include "../Manager.hpp"
#include "../../Infrastructure/Timer.h"

class ScoreManager :public Manager<ScoreManager>
{
	friend class Manager<ScoreManager>;

private:
	Timer levelTimer;                      //从当前规模模拟开启时开始计时

public:
	void OnUpdate(double);

	void RestartTimer();                   //重置计时器

	double GetPassTime() const;            //返回当前时间

private:
	ScoreManager() = default;
	~ScoreManager() = default;
};
#endif