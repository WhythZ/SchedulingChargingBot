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
public:
	bool isCursorDragging = false;             //�Ƿ��ڱ������ק��״̬

	bool isCharging = false;                   //�����Ƿ����ڣ����������
	bool isDischarging = false;                //�����Ƿ����ڣ��������ŵ�

protected:
	bool isValid = true;                       //������ͨ���ñ�������ʵ���Ƿ�Ӧ�����ڳ�����

	Vector2 position;                          //����λ��
	Vector2 velocity;                          //�ٶ�����
	double speed = 0;                          //���ʴ�С����λΪ"��Ƭ/��λʱ��"

	SDL_Point size = { TILE_SIZE,TILE_SIZE };  //����ͼ�ߴ�

	double collideRadius = 0;                  //��ײ������ŵ�İ뾶��Χ

	#pragma region Animation
	Animation* animCurrent;                    //��ǰ��Ⱦ�Ķ���
	Animation animIdling;                      //���ö���
	Animation animCharging;                    //��綯��
	Animation animDischarging;                 //�ŵ綯��
	#pragma endregion

	#pragma region Electricity
	double maximumElectricity = 100;           //���������������ʼ��ʱȷ��
	double currentElectricity;                 //��ǰ����

	double chargingCooldown = 0.1;             //�����ȴ���ʱ��
	double chargingIntensity = 0.5;            //ÿ������ȴ�������ٵ���
	Timer chargingTimer;                       //ʵʩ�����

	double dischargingCooldown = 0.2;          //�ŵ���ȴ���ʱ��
	double dischargingIntensity = 0.5;         //ÿ������ȴ��������ٵ���
	Timer dischargingTimer;                    //ʵʩ�ŵ���
	#pragma endregion

	#pragma region Color
	SDL_Color barColor = { 255,200,0,255 };    //����������ɫ
	SDL_Color linkColor = { 255,255,0,255 };   //�����ߵ���ɫ
	#pragma endregion

public:
	Chargeable();
	~Chargeable() = default;
	void SetPosition(int, int);                //���þ��嵽���ص�λ��
	void SetVelocity(Vector2);                 //�����ٶ�

	virtual void OnUpdate(double);             //ÿ֡��������
	void OnRender(SDL_Renderer*);              //ÿ֡��Ⱦ����

	void Invalidate();                         //����Ϊ���Ϸ����ȴ����������

	bool IsValid() const;                      //��ѯ�Ϸ�״̬
	Vector2 GetPosition() const;               //��ȡ����λ��

protected:
	void UpdateIdling(double);                 //��������ʱ����
	void UpdateCharging(double);               //���³��ʱ����
	void UpdateDischarging(double);            //���·ŵ�ʱ����
};

#endif
