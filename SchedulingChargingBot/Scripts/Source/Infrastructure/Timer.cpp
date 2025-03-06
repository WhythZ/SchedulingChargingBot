#include "../../Header/Infrastructure/Timer.h"

void Timer::SetWaitTime(double _wait)
{
	waitTime = _wait;
}

void Timer::SetOneShot(bool _flag)
{
	isOneShot = _flag;
}

void Timer::SetTimeOutTrigger(std::function<void()> _callback)
{
	trigger = _callback;
}

void Timer::OnUpdate(double _delta)
{
	//检测是否应当暂停
	if (isPaused) return;

	//以传入时间累积时间
	passTime += _delta;

	//若达到了等待时间，计时器就被触发了
	if (passTime >= waitTime)
	{
		//若能被多次触发，或者只能触发一次且没被触发过，则该计时器才可以被触发
		bool _canShot = (!isOneShot || (isOneShot && !isShotted));

		//如果可触发，且回调函数存在，则触发回调函数
		if (_canShot && trigger)
			trigger();

		//消费掉一个等待时间周期，以便下一次触发
		passTime -= waitTime;
	}
}

void Timer::Pause()
{
	isPaused = true;
}

void Timer::Resume()
{
	isPaused = false;
}

void Timer::Restart()
{
	//归零已等待时间
	passTime = 0;
	//重置已被触发过的状态
	isShotted = false;
}

double Timer::GetPassTime() const
{
	return passTime;
}