#include "../../../Header/Manager/Concrete/RobotManager.h"

RobotManager::~RobotManager()
{
	for (Robot* _robot : robotList)
		delete _robot;
}

void RobotManager::SpawnAt(SDL_Point _point)
{
	Robot* _new = new Robot();

	//根据传入的瓦片地图索引坐标，计算对应位置瓦片中心点位置
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);

	//推送该实例入列表
	robotList.push_back(_new);
}

void RobotManager::OnUpdate(double _delta)
{
	//移除非法实例
	RemoveInvalid();

	for (Robot* _robot : robotList)
		_robot->OnUpdate(_delta);
}

void RobotManager::OnRender(SDL_Renderer* _renderer)
{
	for (Robot* _robot : robotList)
		_robot->OnRender(_renderer);
}

void RobotManager::RemoveInvalid()
{
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
	auto _begin = std::remove_if(robotList.begin(), robotList.end(),
		[](const Robot* _robot)
		{
			if (!_robot->IsValid())
			{
				delete _robot;
				return true;
			}
			return false;
		});

	//删除所有无效的子弹，此时的列表在remove_if的排列下，所有无效的子弹指针均在列表末尾
	robotList.erase(_begin, robotList.end());
}

std::vector<Robot*> RobotManager::GetRobotList() const
{
	return robotList;
}