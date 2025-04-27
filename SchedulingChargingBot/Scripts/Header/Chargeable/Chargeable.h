#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include <map>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
#include "../Infrastructure/Animation.h"
#include "../Tilemap/Tile.h"

//������࣬���������ˡ�����
class Chargeable
{
public:
	bool isCursorDragging = false;                      //�Ƿ��ڱ������ק��״̬

	bool isCharged = false;                             //�Ƿ����ڣ����������
	bool isCharger = false;                             //�Ƿ����ڣ��������ŵ�

protected:
	bool isValid = true;                                //������ͨ���ñ�������ʵ���Ƿ�Ӧ�����ڳ�����

	Vector2 position;                                   //����λ��
	Vector2 velocity;                                   //�ٶ�����
	double speed = 0;                                   //���ʴ�С����λΪ"��Ƭ/��λʱ��"
	SDL_Point size = { TILE_SIZE,TILE_SIZE };           //����ͼ�ߴ�

	#pragma region Electricity
	double currentElectricityRatio = 0;                 //��ǰ�����ٷֱ�[0,1]
	
	double chargedCooldown = 0.1;                       //ÿ�Σ�������������ȴ���ʱ��
	double chargedIntensity = 0.01;                     //ÿ�Σ�������������ٰٷֱȵ���
	Timer chargedTimer;                                 //ʵʩ�������������
	
	double chargerCooldown = 0.1;                       //ÿ�Σ��������ŵ����ȴ���ʱ��
	double chargerIntensity = 0.01;                     //ÿ�Σ�������������ٰٷֱȵ���
	Timer chargerTimer;                                 //ʵʩ���������ŵ���
	#pragma endregion

	#pragma region Animation
	Animation* animCurrent;                             //��ǰ��Ⱦ�Ķ���

	Animation animIdle;                                 //���ö���
	Animation animCharged;                              //����������綯��
	Animation animCharger;                              //���������ŵ綯��
	#pragma endregion

	#pragma region Color
	SDL_Color barContentColor = { 255,200,0,255 };      //��������������ɫ
	SDL_Color barBackgroundColor = { 200,200,200,255 }; //�������ı�����ɫ
	SDL_Color linkColor = { 255,255,0,255 };	        //�����ߵ���ɫ
	#pragma endregion

public:
	Chargeable();
	~Chargeable() = default;

	virtual void OnUpdate(double);                      //ÿ֡��������
	void OnRender(SDL_Renderer*);                       //ÿ֡��Ⱦ����

	void SetPosition(int, int);                         //���þ��嵽���ص�λ��
	void SetVelocity(Vector2);                          //�����˶��ٶ�
	void Invalidate();                                  //����Ϊ���Ϸ����ȴ����������
	virtual void ChangeState(std::string) = 0;          //���µ�ǰ״̬

	bool IsValid() const;                               //��ѯ�Ϸ�״̬
	Vector2 GetPosition() const;                        //��ȡ����λ��
	bool HaveElectricity() const;
	bool NeedElectricity() const;
	bool IsInRectArea(const SDL_Rect&) const;           //����Ƿ���������
	bool IsInRectsArea(const std::map<size_t, SDL_Rect>&) const;

protected:
	void UpdateIdling(double);                          //��������ʱ����
	void UpdateCharged(double);                         //���£����������ʱ����
	void UpdateCharger(double);                         //���£��������ŵ�ʱ����
};

#endif
