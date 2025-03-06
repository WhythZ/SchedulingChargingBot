#ifndef _TIMER_H_
#define _TIMER_H_

//֧�ֻص�������Callback�����������������ݵĺ�������ͷ�ļ�
#include <functional>

class Timer
{
private:
	double passTime = 0;                            //�Ӽ�ʱ���������󣬹�ȥ�˶೤ʱ��
	double waitTime = 0;                            //�ȴ��೤ʱ��󴥷�ָ������

	bool isPaused = false;                          //��ʱ������ͣ����ͣ�ڼ�passTime���ۻ�ʱ��
	bool isShotted = false;                         //ֻҪ��ʱ������ʱ����һ�Σ���ֵ�ͱ�Ϊ��
	bool isOneShot = false;                         //true��ʾ�ü�ʱ�����ܴ���һ��
	std::function<void()> trigger;                  //��ʱ����ʱ����������ʱ������ʲô�ص�����

public:
	Timer() = default;
	~Timer() = default;
	void SetWaitTime(double);                       //���õȴ�ʱ��
	void SetOneShot(bool);                          //�����Ƿ���Զ�δ���
	void SetTimeOutTrigger(std::function<void()>);  //����<void()>���޲��޷��Ļص�����

	void OnUpdate(double);                          //���¼�ʱ
	void Pause();                                   //��ͣ��ʱ
	void Resume();                                  //������ʱ
	void Restart();                                 //��ȫ���ü�ʱ״̬

	double GetPassTime() const;                     //��ȡ�Ѿ���ȥ��ʱ��
};

#endif