#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <SDL.h>
#include "../Manager.hpp"
#include "../../Tilemap/Map.h"
#include "../../../Header/Manager/Concrete/VehicleSpawner.h"

//游戏主管理器
class GameManager :public Manager<GameManager>
{
	friend class Manager<GameManager>;

private:
	SDL_Window* window = nullptr;              //存储游戏主窗口，用于显示游戏画面
	SDL_Renderer* renderer = nullptr;          //存储渲染器，渲染器可以使用GPU进行加速
	SDL_Event event;                           //用于调用SDL事件，根据不同的事件类型做出相应的反应
	bool isQuit = false;                       //决定游戏主循环是否结束
	int fps = 120;                             //需要维持的游戏帧率，即每秒刷新的帧数

	VehicleSpawner vehicleSpawner;

public:
	int Run(int, char**);

private:
	GameManager();                   
	~GameManager();                  
	void InitAssert(bool, const char*);        //初始化断言，用于初始化，并依据初始化函数返回的参数判断是否初始化成功
	
	void OnInput();                            //主循环内检测输入事件并作出相应的反应
	void OnUpdate(double);                     //主循环内逐帧检测数据的更新
	void OnRender();                           //主循环内渲染绘图的具体过程
};

#endif
