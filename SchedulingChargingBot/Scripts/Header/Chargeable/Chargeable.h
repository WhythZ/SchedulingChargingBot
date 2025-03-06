#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
#include "../Infrastructure/Animation.h"

//�ɳ������������ؾߡ������ˡ����
class Chargeable
{
protected:
	Vector2 position;                          //����λ��

	#pragma region Setting
	SDL_Point size = { 64,64 };                //����ͼ�ߴ磬Ҳ������ײ��

	double maximumElectricity = 0;             //���������������ʼ��ʱȷ��
	#pragma endregion

	#pragma region Animation
	Animation* animCurrent;                    //��ǰ����
	Animation animIdle;                        //���ö���
	Animation animCharge;                      //����ɶ���
	#pragma endregion

	#pragma region Charging
	double currentElectricity;                 //��ǰ����

	bool isCharging = false;                   //�����Ƿ����ڣ����������
	double chargingIntensity = 0;              //ÿ������ȴ�������ٵ���
	double chargingCooldown = 0;               //�����ȴ���ʱ��
	Timer chargingTimer;                       //ʵʩ�����

	bool isDischarging = false;                //�����Ƿ����ڣ��������ŵ�
	double dischargingCooldown = 0;            //ÿ������ȴ��������ٵ���
	double dischargingIntensity = 0;           //�����ȴ���ʱ��
	Timer dischargingTimer;                    //ʵʩ�ŵ���
	#pragma endregion

public:
	Chargeable() = default;
	~Chargeable() = default;

	virtual void OnUpdate(double);             //ÿ֡��������
	void OnRender(SDL_Renderer*);              //ÿ֡��Ⱦ����

protected:
	void UpdateCharging();                     //���³������
	void UpdateDischarging();                  //���·ŵ�����
};

#endif
