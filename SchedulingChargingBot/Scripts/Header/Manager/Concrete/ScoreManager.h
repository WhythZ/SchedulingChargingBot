#ifndef _SCORE_MANAGER_H_
#define _SCORE_MANAGER_H_

#include <SDL.h>
#include "../Manager.hpp"
#include "../../Infrastructure/Timer.h"

class ScoreManager :public Manager<ScoreManager>
{
	friend class Manager<ScoreManager>;

private:
	Timer levelTimer;                      //�ӵ�ǰ��ģģ�⿪��ʱ��ʼ��ʱ

public:
	void OnUpdate(double);

	void RestartTimer();                   //���ü�ʱ��

	double GetPassTime() const;            //���ص�ǰʱ��

private:
	ScoreManager() = default;
	~ScoreManager() = default;
};
#endif