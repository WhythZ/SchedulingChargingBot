#include "../../../Header/Manager/Concrete/RobotManager.h"

RobotManager::~RobotManager()
{
	for (Robot* _robot : robotList)
		delete _robot;
}

void RobotManager::SpawnAt(SDL_Point _point)
{
	Robot* _new = new Robot();

	//���ݴ������Ƭ��ͼ�������꣬�����Ӧλ����Ƭ���ĵ�λ��
	int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
	int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
	_new->SetPosition(_x, _y);

	//���͸�ʵ�����б�
	robotList.push_back(_new);
}

void RobotManager::OnUpdate(double _delta)
{
	//�Ƴ��Ƿ�ʵ��
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
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ�����б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
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

	//ɾ��������Ч���ӵ�����ʱ���б���remove_if�������£�������Ч���ӵ�ָ������б�ĩβ
	robotList.erase(_begin, robotList.end());
}

std::vector<Robot*> RobotManager::GetRobotList() const
{
	return robotList;
}