#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
//#include "../Infrastructure/Animation.h"

//可充电机器，派生载具、机器人、电池
class Chargeable
{
protected:
	#pragma region Setting
	SDL_Point size;                            //碰撞体积
	double speed;                              //速率大小
	#pragma endregion

	Vector2 position;                          //中心位置
	Vector2 velocity;                          //速度向量

	SDL_Texture* texture;                      //纹理图片

	#pragma region Charging
	double maximumElectricity;                 //最大电量
	double currentElectricity;                 //当前电量

	bool isCharging = false;                   //控制是否正在（被动）充电
	bool isDischarging = false;                //控制是否正在（主动）放电

	Timer chargingTimer;                       //控制充电间隔
	Timer dischargingTimer;                    //控制放电间隔
	#pragma endregion

public:
	Chargeable() = default;
	~Chargeable() = default;

	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);

protected:
	void UpdateCharging();                     //更新充电
	void UpdateDischarging();                  //更新放电
};

#endif
