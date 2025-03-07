#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
#include "../Infrastructure/Animation.h"
#include "../Tilemap/Tile.h"

//可充电机器，派生载具、机器人、电池
class Chargeable
{
protected:
	bool isValid = true;                       //管理器通过该变量管理实例是否应当处于场景中

	#pragma region Transform
	Vector2 position;                          //中心位置
	SDL_Point size = { TILE_SIZE,TILE_SIZE };  //纹理图尺寸，也用作碰撞箱
	//相对位置，覆盖右侧，用于填充电量条
	SDL_Rect barRect = { TILE_SIZE/2,0,TILE_SIZE/2,TILE_SIZE };
	#pragma endregion

	#pragma region Animation
	Animation* animCurrent;                    //当前渲染的动画
	Animation animIdling;                      //闲置动画
	Animation animCharging;                    //充电动画
	Animation animDischarging;                 //放电动画
	#pragma endregion

	#pragma region Electricity
	double maximumElectricity = 100;           //最大电量，在子类初始化时确定
	double currentElectricity;                 //当前电量

	bool isCharging = false;                   //控制是否正在（被动）充电
	double chargingCooldown = 0.1;             //充电冷却间隔时间
	double chargingIntensity = 0.5;            //每经过冷却后充入多少电量
	Timer chargingTimer;                       //实施充电间隔

	bool isDischarging = false;                //控制是否正在（主动）放电
	double dischargingCooldown = 0.2;          //每经过冷却后输出多少电量
	double dischargingIntensity = 0.5;         //充电冷却间隔时间
	Timer dischargingTimer;                    //实施放电间隔
	#pragma endregion

public:
	Chargeable();
	~Chargeable() = default;
	void SetPosition(int, int);                //设置具体到像素的位置

	virtual void OnUpdate(double);             //每帧更新数据
	void OnRender(SDL_Renderer*);              //每帧渲染动画

	void Invalidate();                         //设置为不合法，等待管理器清除

	bool IsValid() const;                      //查询合法状态

protected:
	void UpdateIdling(double);                 //更新闲置时数据
	void UpdateCharging(double);               //更新充电时数据
	void UpdateDischarging(double);            //更新放电时数据
};

#endif
