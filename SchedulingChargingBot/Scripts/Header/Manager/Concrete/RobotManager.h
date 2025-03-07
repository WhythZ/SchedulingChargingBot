#ifndef _ROBOT_MANAER_H_
#define _ROBOT_MANAER_H_

#include "../Manager.hpp"
#include <vector>
#include "../../Chargeable/Concrete/Robot.h"

class RobotManager :public Manager<RobotManager>
{
	friend class Manager<RobotManager>;

public:
	void SpawnAt(SDL_Point);                 //以瓦片地图的瓦片索引间接实例化该实体

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

private:
	std::vector<Robot*> robotList;

private:
	RobotManager() = default;
	~RobotManager();

	void RemoveInvalid();                    //移除非法实例
};

#endif
