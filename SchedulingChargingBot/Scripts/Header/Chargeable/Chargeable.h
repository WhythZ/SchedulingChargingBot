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
public:
	bool isCursorDragging = false;                      //是否处于被鼠标拖拽的状态

	bool isCharging = false;                            //控制是否正在（被动）充电
	bool isDischarging = false;                         //控制是否正在（主动）放电

protected:
	bool isValid = true;                                //管理器通过该变量管理实例是否应当处于场景中

	Vector2 position;                                   //中心位置
	Vector2 velocity;                                   //速度向量
	double speed = 0;                                   //速率大小，单位为"瓦片/单位时间"

	SDL_Point size = { TILE_SIZE,TILE_SIZE };           //纹理图尺寸

	double collideRadius = 0;                           //碰撞触发充放电的半径范围

	#pragma region Animation
	Animation* animCurrent;                             //当前渲染的动画
	Animation animIdling;                               //闲置动画
	Animation animCharging;                             //充电动画
	Animation animDischarging;                          //放电动画
	#pragma endregion

	#pragma region Electricity
	double currentElectricityRatio = 0;                 //当前电量百分比[0,1]

	double chargingCooldown = 0.1;                      //充电冷却间隔时间
	double chargingIntensity = 0.01;                    //每经过冷却后充入多少百分比电量
	Timer chargingTimer;                                //实施充电间隔

	double dischargingCooldown = 0.1;                   //放电冷却间隔时间
	double dischargingIntensity = 0.01;                 //每经过冷却后输出多少百分比电量
	Timer dischargingTimer;                             //实施放电间隔
	#pragma endregion

	#pragma region Color
	SDL_Color barContentColor = { 255,200,0,255 };      //电量条的内容颜色
	SDL_Color barBackgroundColor = { 200,200,200,255 }; //电量条的背景颜色
	SDL_Color linkColor = { 255,255,0,255 };	        //连接线的颜色
	#pragma endregion

public:
	Chargeable();
	~Chargeable() = default;
	void SetPosition(int, int);                         //设置具体到像素的位置
	void SetVelocity(Vector2);                          //更新速度

	virtual void OnUpdate(double);                      //每帧更新数据
	void OnRender(SDL_Renderer*);                       //每帧渲染动画

	void Invalidate();                                  //设置为不合法，等待管理器清除

	bool IsValid() const;                               //查询合法状态
	Vector2 GetPosition() const;                        //获取中心位置

protected:
	void UpdateIdling(double);                          //更新闲置时数据
	void UpdateCharging(double);                        //更新充电时数据
	void UpdateDischarging(double);                     //更新放电时数据
};

#endif
