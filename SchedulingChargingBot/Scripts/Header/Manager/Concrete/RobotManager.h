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
	void SpawnAt(SDL_Point);                   //����Ƭ��ͼ����Ƭ�������ʵ������ʵ��

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	std::vector<Robot*> GetRobotList() const;  //��ȡʵ���б�

private:
	RobotManager() = default;
	~RobotManager();

	void RemoveInvalid();                      //�Ƴ��Ƿ�ʵ��
};

#endif
