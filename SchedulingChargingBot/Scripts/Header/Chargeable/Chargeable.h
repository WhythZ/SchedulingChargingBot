#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
#include "../Infrastructure/Animation.h"

//可充电机器，派生载具、机器人、电池
class Chargeable
{
protected:
	Vector2 position;                          //中心位置

	#pragma region Setting
	SDL_Point size = { 64,64 };                //纹理图尺寸，也用作碰撞箱

	double maximumElectricity = 0;             //最大电量，在子类初始化时确定
	#pragma endregion

	#pragma region Animation
	Animation* animCurrent;                    //当前动画
	Animation animIdle;                        //闲置动画
	Animation animCharge;                      //带电荷动画
	#pragma endregion

	#pragma region Charging
	double currentElectricity;                 //当前电量

	bool isCharging = false;                   //控制是否正在（被动）充电
	double chargingIntensity = 0;              //每经过冷却后充入多少电量
	double chargingCooldown = 0;               //充电冷却间隔时间
	Timer chargingTimer;                       //实施充电间隔

	bool isDischarging = false;                //控制是否正在（主动）放电
	double dischargingCooldown = 0;            //每经过冷却后输出多少电量
	double dischargingIntensity = 0;           //充电冷却间隔时间
	Timer dischargingTimer;                    //实施放电间隔
	#pragma endregion

public:
	Chargeable() = default;
	~Chargeable() = default;

	virtual void OnUpdate(double);             //每帧更新数据
	void OnRender(SDL_Renderer*);              //每帧渲染动画

protected:
	void UpdateCharging();                     //更新充电数据
	void UpdateDischarging();                  //更新放电数据
};

#endif
