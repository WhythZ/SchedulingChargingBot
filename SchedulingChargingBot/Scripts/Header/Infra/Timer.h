#ifndef _TIMER_H_
#define _TIMER_H_

//支持回调函数（Callback，即被当作参数传递的函数）的头文件
#include <functional>

class Timer
{
private:
	double passTime = 0;                            //从计时器被创建后，过去了多长时间
	double waitTime = 0;                            //等待多长时间后触发指定功能

	bool isPaused = false;                          //计时器的暂停，暂停期间passTime不累积时间
	bool isShotted = false;                         //只要计时器被到时触发一次，该值就变为真
	bool isOneShot = false;                         //true表示该计时器仅能触发一次
	std::function<void()> trigger;                  //计时器计时结束被触发时，触发什么回调函数

public:
	Timer() = default;
	~Timer() = default;
	void SetWaitTime(double);                       //设置等待时间
	void SetOneShot(bool);                          //设置是否可以多次触发
	void SetTimeOutTrigger(std::function<void()>);  //设置<void()>即无参无返的回调函数

	void OnUpdate(double);                          //更新计时
	void Pause();                                   //暂停计时
	void Resume();                                  //继续计时
	void Restart();                                 //完全重置计时状态

	double GetPassTime() const;                     //获取已经过去的时间
};

#endif