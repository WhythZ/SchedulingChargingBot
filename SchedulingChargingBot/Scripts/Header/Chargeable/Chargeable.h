#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
#include "../Infrastructure/Animation.h"
#include "../Tilemap/Tile.h"

//�ɳ������������ؾߡ������ˡ����
class Chargeable
{
protected:
	bool isValid = true;                       //������ͨ���ñ�������ʵ���Ƿ�Ӧ�����ڳ�����

	#pragma region Transform
	Vector2 position;                          //����λ��
	SDL_Point size = { TILE_SIZE,TILE_SIZE };  //����ͼ�ߴ磬Ҳ������ײ��
	//���λ�ã������Ҳ࣬������������
	SDL_Rect barRect = { TILE_SIZE/2,0,TILE_SIZE/2,TILE_SIZE };
	#pragma endregion

	#pragma region Animation
	Animation* animCurrent;                    //��ǰ��Ⱦ�Ķ���
	Animation animIdling;                      //���ö���
	Animation animCharging;                    //��綯��
	Animation animDischarging;                 //�ŵ綯��
	#pragma endregion

	#pragma region Electricity
	double maximumElectricity = 100;           //���������������ʼ��ʱȷ��
	double currentElectricity;                 //��ǰ����

	bool isCharging = false;                   //�����Ƿ����ڣ����������
	double chargingCooldown = 0.1;             //�����ȴ���ʱ��
	double chargingIntensity = 0.5;            //ÿ������ȴ�������ٵ���
	Timer chargingTimer;                       //ʵʩ�����

	bool isDischarging = false;                //�����Ƿ����ڣ��������ŵ�
	double dischargingCooldown = 0.2;          //ÿ������ȴ��������ٵ���
	double dischargingIntensity = 0.5;         //�����ȴ���ʱ��
	Timer dischargingTimer;                    //ʵʩ�ŵ���
	#pragma endregion

public:
	Chargeable();
	~Chargeable() = default;
	void SetPosition(int, int);                //���þ��嵽���ص�λ��

	virtual void OnUpdate(double);             //ÿ֡��������
	void OnRender(SDL_Renderer*);              //ÿ֡��Ⱦ����

	void Invalidate();                         //����Ϊ���Ϸ����ȴ����������

	bool IsValid() const;                      //��ѯ�Ϸ�״̬

protected:
	void UpdateIdling(double);                 //��������ʱ����
	void UpdateCharging(double);               //���³��ʱ����
	void UpdateDischarging(double);            //���·ŵ�ʱ����
};

#endif
