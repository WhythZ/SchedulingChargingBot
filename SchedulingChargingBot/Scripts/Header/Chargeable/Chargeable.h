#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
//#include "../Infrastructure/Animation.h"

//�ɳ������������ؾߡ������ˡ����
class Chargeable
{
protected:
	#pragma region Setting
	SDL_Point size;                            //��ײ���
	double speed;                              //���ʴ�С
	#pragma endregion

	Vector2 position;                          //����λ��
	Vector2 velocity;                          //�ٶ�����

	SDL_Texture* texture;                      //����ͼƬ

	#pragma region Charging
	double maximumElectricity;                 //������
	double currentElectricity;                 //��ǰ����

	bool isCharging = false;                   //�����Ƿ����ڣ����������
	bool isDischarging = false;                //�����Ƿ����ڣ��������ŵ�

	Timer chargingTimer;                       //���Ƴ����
	Timer dischargingTimer;                    //���Ʒŵ���
	#pragma endregion

public:
	Chargeable() = default;
	~Chargeable() = default;

	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);

protected:
	void UpdateCharging();                     //���³��
	void UpdateDischarging();                  //���·ŵ�
};

#endif
