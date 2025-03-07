#ifndef _ROBOT_MANAER_H_
#define _ROBOT_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/Concrete/Robot.h"

class RobotManager :public Manager<RobotManager>
{
	friend class Manager<RobotManager>;

private:
	std::vector<Robot*> robotList;

public:
	void SpawnAt(SDL_Point);                   //以瓦片地图的瓦片索引间接实例化该实体

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	std::vector<Robot*> GetRobotList() const;  //获取实例列表

private:
	RobotManager() = default;
	~RobotManager();

	void RemoveInvalid();                      //移除非法实例
};

#endif
