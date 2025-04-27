#ifndef _CHARGEABLE_H_
#define _CHARGEABLE_H_

#include <SDL.h>
#include <map>
#include "../Infrastructure/Vector2.h"
#include "../Infrastructure/Timer.h"
#include "../Infrastructure/Animation.h"
#include "../Tilemap/Tile.h"

//带电基类，派生机器人、汽车
class Chargeable
{
public:
	bool isCursorDragging = false;                      //是否处于被鼠标拖拽的状态

	bool isCharged = false;                             //是否正在（被动）充电
	bool isCharger = false;                             //是否正在（主动）放电

protected:
	bool isValid = true;                                //管理器通过该变量管理实例是否应当处于场景中

	Vector2 position;                                   //中心位置
	Vector2 velocity;                                   //速度向量
	double speed = 0;                                   //速率大小，单位为"瓦片/单位时间"
	SDL_Point size = { TILE_SIZE,TILE_SIZE };           //纹理图尺寸

	#pragma region Electricity
	double currentElectricityRatio = 0;                 //当前电量百分比[0,1]
	
	double chargedCooldown = 0.1;                       //每次（被动）充电的冷却间隔时间
	double chargedIntensity = 0.01;                     //每次（被动）充入多少百分比电量
	Timer chargedTimer;                                 //实施（被动）充电间隔
	
	double chargerCooldown = 0.1;                       //每次（主动）放电的冷却间隔时间
	double chargerIntensity = 0.01;                     //每次（主动）输出多少百分比电量
	Timer chargerTimer;                                 //实施（主动）放电间隔
	#pragma endregion

	#pragma region Animation
	Animation* animCurrent;                             //当前渲染的动画

	Animation animIdle;                                 //闲置动画
	Animation animCharged;                              //（被动）充电动画
	Animation animCharger;                              //（主动）放电动画
	#pragma endregion

	#pragma region Color
	SDL_Color barContentColor = { 255,200,0,255 };      //电量条的内容颜色
	SDL_Color barBackgroundColor = { 200,200,200,255 }; //电量条的背景颜色
	SDL_Color linkColor = { 255,255,0,255 };	        //连接线的颜色
	#pragma endregion

public:
	Chargeable();
	~Chargeable() = default;

	virtual void OnUpdate(double);                      //每帧更新数据
	void OnRender(SDL_Renderer*);                       //每帧渲染动画

	void SetPosition(int, int);                         //设置具体到像素的位置
	void SetVelocity(Vector2);                          //更新运动速度
	void Invalidate();                                  //设置为不合法，等待管理器清除
	virtual void ChangeState(std::string) = 0;          //更新当前状态

	bool IsValid() const;                               //查询合法状态
	Vector2 GetPosition() const;                        //获取中心位置
	bool HaveElectricity() const;
	bool NeedElectricity() const;
	bool IsInRectArea(const SDL_Rect&) const;           //检测是否在区域内
	bool IsInRectsArea(const std::map<size_t, SDL_Rect>&) const;

protected:
	void UpdateIdling(double);                          //更新闲置时数据
	void UpdateCharged(double);                         //更新（被动）充电时数据
	void UpdateCharger(double);                         //更新（主动）放电时数据
};

#endif
