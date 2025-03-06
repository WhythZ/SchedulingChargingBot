#ifndef _ROBOT_MANAER_H_
#define _ROBOT_MANAER_H_

#include "../Manager.hpp"

class RobotManager :public Manager<RobotManager>
{
	friend class Manager<RobotManager>;
private:
	RobotManager() = default;
	~RobotManager() = default;
};

#endif
