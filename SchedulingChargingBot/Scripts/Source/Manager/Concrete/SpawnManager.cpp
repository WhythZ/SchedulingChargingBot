#include "../../../Header/Manager/Concrete/SpawnManager.h"
#include <set>
#include <iostream>
#include<random>
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"

void SpawnManager::OnUpdate(double _delta)
{
	UpdateVehicleSpawn(_delta);
	UpdateRobotSpawn(_delta);
}

void SpawnManager::ChangeLevel(ScaleLevel _level)
{
    currentScaleLevel = _level;

    //���
    ChargeableManager::Instance()->ClearAll();

    //���¼���
    LoadVehicleLevel(_level);
    LoadRobotLevel(_level);
}

SpawnManager::ScaleLevel SpawnManager::GetCurrentScaleLevel() const
{
    return currentScaleLevel;
}

int SpawnManager::GetTotalSpawned() const
{
    return totalSpawned;
}

int SpawnManager::GetTotalLeft() const
{
    return totalLeft;
}

void SpawnManager::UpdateVehicleSpawn(double _delta)
{
    //�ۼӷ���ʱ��
    elapsedTime += _delta;

    //�ѡ�����ʱ���ѵ����������ɴ����߳���
    while (nextIndex < tasks.size() && tasks[nextIndex].spawnTime <= elapsedTime)
    {
        const auto& t = tasks[nextIndex];

        Vehicle* v = new Vehicle();
        v->SetPosition((int)t.position_spawn.x, (int)t.position_spawn.y);
        v->SetElectricity(t.initialElectricity);
        v->SetTargetElectricity(t.requiredElectricity);
        v->SetLeaveTime(t.leaveTime);
        v->VehicleNo = t.VehicleTaskNo;//��task��vehicle����һһ��Ӧ�������ӵ���֮һ�֣�

        Vector2 initDirection;
        initDirection.x = (t.position.x - t.position_spawn.x);
        initDirection.y = (t.position.y - t.position_spawn.y);//�����볡ʱ����

        v->SetVelocity(initDirection.Normalized() * v->GetSpeed());

        v->isOnline = false;     // �յ����δ��ʽ�������
        v->arriveTime = t.spawnTime;

        pendingQueue.push(v);    // ����ȴ����߶���
        ++nextIndex;
    }
    // ��� pendingQueue ����Щ��������ʽ����
    size_t pendingqueueSize = pendingQueue.size();
    if (pendingqueueSize) for (size_t i = 0; i < pendingqueueSize; ++i) {
        Vehicle* v = pendingQueue.front();
        v->isMoving = true;//����Ϊ�����ƶ�����ֹ����������硣
        pendingQueue.pop();
        if (v->arriveTime <= elapsedTime)
        {
            // ����ʱ������ �� �������ϵͳ
            v->isOnline = true;
            //v->ChangeState("Charged");
            ChargeableManager::Instance()->AddExternalChargeable(ChargeableType::Vehicle, v);

            printf("[ARRIVE] Vehicle at (%.0f, %.0f), elec=%.0f%%, needs=%.0f%%, leave=%.0fs\n",
                v->GetPosition().x, v->GetPosition().y,
                v->GetElectricity(),    // ʹ�� %.0f ��ʽ��Ϊ��С��λ��double
                v->GetTargetElectricity(),
                v->GetLeaveTime());
            comingQueue.push(v);//�����������ĳ�������
        }
        else pendingQueue.push(v);//��������㣬���ٴӶ�β�������
    }
    size_t comingQueueSize = comingQueue.size();
    if (comingQueueSize) for (size_t i = 0; i < comingQueueSize; ++i) {
        Vehicle* v = comingQueue.front();
        comingQueue.pop();
        //std::cout << "comingQueueSize��" << comingQueueSize << std::endl;
        v->IsTouchingMapBorder = false;//��ֹ���ɳ���ʱ������
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//��indexʹ��task��vehicle��Ӧ��ʱ��������
        }
        //std::cout << "v->GetPosition().x" << v->GetPosition().x << std::endl << "tasks[index].position.x" << tasks[index].position.x << std::endl;
        if ((int)v->GetPosition().x == (int)tasks[index].position.x || (int)v->GetPosition().y == (int)tasks[index].position.y) {//�ж��ǲ��ǵ���Ŀ��λ�ã�������Ը����ƣ���������͵������
            v->SetVelocity({ 0,0 });
            v->SetPosition((int)tasks[index].position.x, (int)tasks[index].position.y);
            workingQueue.push(v);
        }
        else comingQueue.push(v);
    }
    size_t workingQueueSize = workingQueue.size();
    if (workingQueueSize) for (size_t i = 0; i < workingQueueSize; ++i) {
        Vehicle* v = workingQueue.front();
        v->isMoving = false;//����Ϊ�����ƶ��У��û���������硣
        workingQueue.pop();
        //std::cout << "workingQueueSize��" << workingQueueSize << std::endl;
        //std::cout << "vehicle current electricity:" << v->GetCurrentElectricity() << std::endl;
        //std::cout << "vehicle target electricity:" << v->GetTargetElectricity() << std::endl;
        //std::cout << "vehicle isCharged :" << v->isCharged << std::endl;
        //std::cout << "vehicle isTargeted :" << v->isTargeted << std::endl;
        //std::cout << "vehicle isbusy :" << v->IsBusy() << std::endl;
        //std::cout << "vehicle isMoving :" << v->rtisMoving() << std::endl;
        //std::cout << "vehicle charger :" << v->charger << std::endl;
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//��indexʹ��task��vehicle��Ӧ��ʱ��������
        }
        if (!v->NeedElectricity() || tasks[index].leaveTime <= elapsedTime) {//�����������������뿪ʱ��ʱ�����뿪���С�
            Vector2 endDirection = { 0 , 0 };
            endDirection.x = (tasks[index].position_leave.x - tasks[index].position.x);
            endDirection.y = (tasks[index].position_leave.y - tasks[index].position.y);
            //�����������뿪ʱ����
            v->SetVelocity(endDirection.Normalized() * v->GetSpeed());//���뿪��λ�ñ�ȥ
            //std::cout << "��ҹ��ȥ" << std::endl;
            leavingQueue.push(v);
        }
        else workingQueue.push(v);
    }
    size_t leavingQueueSize = leavingQueue.size();
    if (leavingQueueSize) for (size_t i = 0; i < leavingQueueSize; ++i) {
        //std::cout << "leavingQueueSize:" << leavingQueueSize << std::endl;
        Vehicle* v = leavingQueue.front();
        v->isMoving = true;//����Ϊ�����ƶ�����ֹǱ�����⡣
        leavingQueue.pop();
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//��indexʹ��task��vehicle��Ӧ��ʱ��������
        }
        //std::cout << "�����߽磿" << v->IsTouchingMapBorder << std::endl;
        if (v->IsTouchingMapBorder == true) {
            v->Invalidate();//����Ϊinvalid���ȴ�manager�����
        }
        else leavingQueue.push(v);
    }
}

void SpawnManager::UpdateRobotSpawn(double _delta)
{
}

void SpawnManager::LoadVehicleLevel(ScaleLevel _level)
{
    //��վ�����
    tasks.clear();
    //��������
    nextIndex = 0;
    //���÷���ʱ��
    elapsedTime = 0;

    //��յȴ�����
    while (!pendingQueue.empty()) pendingQueue.pop();

    //�����Ѷȵȼ�����Ҫ���ɵĳ�������
    int vehicleCount = 0;
    switch (_level)
    {
    case SpawnManager::ScaleLevel::Small:
        vehicleCount = 10;
        break;
    case SpawnManager::ScaleLevel::Medium:
        vehicleCount = 50;
        break;
    case SpawnManager::ScaleLevel::Large:
        vehicleCount = 100;
        break;
    default:
        break;
    }

    //��̬��ȡ��ͼ����Ƭ��������������
    int mapTilesX = SceneManager::Instance()->mapRect.w / TILE_SIZE;
    int mapTilesY = SceneManager::Instance()->mapRect.h / TILE_SIZE;

    //��¼������Ƭ�������ظ�����
    std::set<std::pair<int, int>> occupiedTiles;

    for (int i = 0; i < vehicleCount; ++i) {
        VehicleSpawnTask task;
        //��ǳ�����ţ����ڱ�ʶÿһ����
        task.VehicleTaskNo = i;

        //���ó�������ʱ�䣺���Ƴٲ�������Ŷ������⼯������
        task.spawnTime = rand() % 10 + i * 3;

        int tileX, tileY;
        int tile_initX, tile_initY;
        int tile_endX, tile_endY;
        //���ѡ��δռ�õ���Ƭ���ܿ���ͼ��Ե2��
        //��ʼλ�ú�����λ�ö����ڵ�ͼ��Ե
        do
        {
            tileX = rand() % (mapTilesX - 8) + 4;
            tileY = rand() % (mapTilesY - 8) + 4;

            //��������߼�
            tile_initX = rand() % mapTilesX;
            tile_initY = rand() % mapTilesY;

            //ѡ���ǿ�X�߽绹��Y�߽�
            int a = rand() % 2;

            tile_initX = a ? (rand() % 2) * (mapTilesX - 1) : tile_initX; //X=0��X=max
            tile_initY = a ? tile_initY : (rand() % 2) * (mapTilesY - 1); //Y=0��Y=max

            //�յ������߼�
            tile_endX = rand() % mapTilesX;
            tile_endY = rand() % mapTilesY;

            //ѡ���ǿ�X�߽绹��Y�߽�
            int b = rand() % 2;

            tile_endX = b ? (rand() % 2) * (mapTilesX - 1) : tile_endX;
            tile_endY = b ? tile_endY : (rand() % 2) * (mapTilesY - 1);

        } while (occupiedTiles.count({ tileX, tileY }) && occupiedTiles.count({ tile_initX, tile_initY }) && occupiedTiles.count({ tile_endX, tile_endY }));  // ȷ�����ظ�

        //��¼��Щ��Ƭ�ѱ�ʹ��
        occupiedTiles.insert({ tileX, tileY });
        occupiedTiles.insert({ tile_initX, tile_initY });
        occupiedTiles.insert({ tile_endX, tile_endY });

        //���ó�����ʵ��λ�á���ʼλ�ú��뿪λ�ã�ת��Ϊ�������꣬��������Ƭ��
        task.position =
        {
            static_cast<double>(tileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tileY * TILE_SIZE + TILE_SIZE / 2)
        };

        task.position_spawn =
        {
            static_cast<double>(tile_initX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tile_initY * TILE_SIZE + TILE_SIZE / 2)
        };

        task.position_leave =
        {
            static_cast<double>(tile_endX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tile_endY * TILE_SIZE + TILE_SIZE / 2)
        };

        //��ʼ����Ϊ 20%~59%
        task.initialElectricity = rand() % 40 + 20;

        //�뿪ʱҪ��ĵ���Ϊ 80%~100%
        task.requiredElectricity = 80 + rand() % 21;

        //�뿪ʱ��Ϊ����ʱ��� 60~120 ��
        task.leaveTime = task.spawnTime + 60 + rand() % 61;

        //���������б�
        tasks.push_back(task);
    }
}

void SpawnManager::LoadRobotLevel(ScaleLevel _level)
{
    //�����Ѷȵȼ�����Ҫ���ɵĻ�������
    int _robotCount = 0;
    switch (_level)
    {
    case SpawnManager::ScaleLevel::Small:
        _robotCount = 4;
        break;
    case SpawnManager::ScaleLevel::Medium:
        _robotCount = 8;
        break;
    case SpawnManager::ScaleLevel::Large:
        _robotCount = 12;
        break;
    default:
        break;
    }

    for (int _i = 0; _i < _robotCount; _i++)
        ChargeableManager::Instance()->SpawnChargeableAt(ChargeableType::Robot, { 0,0 });
}