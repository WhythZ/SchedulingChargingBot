#include "../../../Header/Manager/Concrete/SpawnManager.h"
#include <set>
#include <iostream>
#include<random>
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ScoreManager.h"

void SpawnManager::OnUpdate(double _delta)
{
	UpdateVehicleSpawn(_delta);
	//UpdateRobotSpawn(_delta);
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

SpawnManager::ScaleLevel SpawnManager::GetCurrentLevel() const
{
    return currentScaleLevel;
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
        v->SetPosition((int)t.spawnPosition.x, (int)t.spawnPosition.y);
        v->SetElectricity(t.initialElectricity);
        v->SetTargetElectricity(t.requiredElectricity);
        v->SetLeaveTime(t.leaveTime);
        v->VehicleNo = t.VehicleTaskNo;//��task��vehicle����һһ��Ӧ�������ӵ���֮һ�֣�

        Vector2 initDirection;
        initDirection.x = (t.targetPosition.x - t.spawnPosition.x);
        initDirection.y = (t.targetPosition.y - t.spawnPosition.y);//�����볡ʱ����

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
        if ((int)v->GetPosition().x == (int)tasks[index].targetPosition.x || (int)v->GetPosition().y == (int)tasks[index].targetPosition.y) {//�ж��ǲ��ǵ���Ŀ��λ�ã�������Ը����ƣ���������͵������
            v->SetVelocity({ 0,0 });
            v->SetPosition((int)tasks[index].targetPosition.x, (int)tasks[index].targetPosition.y);
            workingQueue.push(v);
        }
        else comingQueue.push(v);
    }

    size_t workingQueueSize = workingQueue.size();    
    if (workingQueueSize) for (size_t i = 0; i < workingQueueSize; ++i) {
        Vehicle* v = workingQueue.front();
        v->isMoving = false;//����Ϊ�����ƶ��У��û���������硣
        workingQueue.pop();
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//��indexʹ��task��vehicle��Ӧ��ʱ��������
        }
        if (!v->NeedElectricity() || tasks[index].leaveTime <= elapsedTime) {//�����������������뿪ʱ��ʱ�����뿪���С�
            Vector2 endDirection = { 0 , 0 };
            endDirection.x = (tasks[index].leavePosition.x - tasks[index].targetPosition.x);
            endDirection.y = (tasks[index].leavePosition.y - tasks[index].targetPosition.y);
            //�����������뿪ʱ����
            v->SetVelocity(endDirection.Normalized() * v->GetSpeed());//���뿪��λ�ñ�ȥ
            //std::cout << "��ҹ��ȥ" << std::endl;
            leavingQueue.push(v);
        }
        else workingQueue.push(v);
    }
    
    size_t leavingQueueSize = leavingQueue.size();  
    if (leavingQueueSize) for (size_t i = 0; i < leavingQueueSize; ++i)
    {
        Vehicle* v = leavingQueue.front();
        //����Ϊ�����ƶ�����ֹǱ������
        v->isMoving = true;
        leavingQueue.pop();
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            //��indexʹ��task��vehicle��Ӧ��ʱ������
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;
        }
        if (v->IsTouchingMapBorder == true)
            v->Invalidate();
        else leavingQueue.push(v);
    }
}

void SpawnManager::LoadVehicleLevel(ScaleLevel _level)
{
    //���õ�ǰ�����еĳ�����ر���
    RefreshVehicleTasks();

    #pragma region VehicleCount
    int _vehicleCount = 0;
    switch (_level)
    {
    case SpawnManager::ScaleLevel::Small:
        _vehicleCount = 10;
        break;
    case SpawnManager::ScaleLevel::Medium:
        _vehicleCount = 50;
        break;
    case SpawnManager::ScaleLevel::Large:
        _vehicleCount = 100;
        break;
    default:
        break;
    }
    #pragma endregion

    size_t _mapTilesX = SceneManager::Instance()->map.GetWidthTileNum();
    size_t _mapTilesY = SceneManager::Instance()->map.GetHeightTileNum();

    //������������ؾ���������
    for (int _i = 0; _i < _vehicleCount; _i++)
    {
		//����һ���ؾ���������
        VehicleSpawnTask _task;
		//�ؾ����������Ψһ��ʶ��
        _task.VehicleTaskNo = _i;
        
        #pragma region RandomArriveTime
        _task.spawnTime = rand() % 5 + _i * 2;
        #pragma endregion

        #pragma region RandomPosition
        size_t _targetTileX = 0, _targetTileY = 0;
        size_t _spawnTileX = 0, _spawnTileY = 0;
        size_t _leaveTileX = 0, _leaveTileY = 0;

		//��Ŀ��λ�ô���������ͼ���ڰ���2����Ƭ��λ��������
        _targetTileX = rand() % (_mapTilesX - 4) + 2;
        _targetTileY = rand() % (_mapTilesY - 4) + 2;

		//�ؾ��ڱ�Ե���ֺ��뿪��λ�ã������һ���ߣ�0�ϣ�1�£�2��3��
        size_t _edge = rand() % 4;
        switch (_edge)
        {
        case 0: //�ϱ�
            _spawnTileX = rand() % _mapTilesX;
            _spawnTileY = 0;
            _leaveTileX = rand() % _mapTilesX;
            _leaveTileY = 0;
            break;
        case 1: //�±�
            _spawnTileX = rand() % _mapTilesX;
            _spawnTileY = _mapTilesY - 1;
            _leaveTileX = rand() % _mapTilesX;
            _leaveTileY = _mapTilesY - 1;
            break;
        case 2: //���
            _spawnTileX = 0;
            _spawnTileY = rand() % _mapTilesY;
            _leaveTileX = 0;
            _leaveTileY = rand() % _mapTilesY;
            break;
        case 3: //�ұ�
            _spawnTileX = _mapTilesX - 1;
            _spawnTileY = rand() % _mapTilesY;
            _leaveTileX = _mapTilesX - 1;
            _leaveTileY = rand() % _mapTilesY;
            break;
        }
        
        //�����������ɵ�����
        _task.targetPosition =
        {
            static_cast<double>(_targetTileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(_targetTileY * TILE_SIZE + TILE_SIZE / 2)
        };
        _task.spawnPosition =
        {
            static_cast<double>(_spawnTileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(_spawnTileY * TILE_SIZE + TILE_SIZE / 2)
        };
        _task.leavePosition =
        {
            static_cast<double>(_leaveTileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(_leaveTileY * TILE_SIZE + TILE_SIZE / 2)
        };
        #pragma endregion

        #pragma region RandomElectricityDemand
        //��ʼ�����������������
        _task.initialElectricity = rand() % 40 + 20;
        _task.requiredElectricity = 80 + rand() % 21;
        _task.leaveTime = _task.spawnTime + 60 + rand() % 61;
        #pragma endregion

        tasks.push_back(_task);
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

void SpawnManager::RefreshVehicleTasks()
{
    //����ؾ������������
    tasks.clear();
    while (!pendingQueue.empty()) pendingQueue.pop();
    while (!comingQueue.empty()) comingQueue.pop();
    while (!workingQueue.empty()) workingQueue.pop();
    while (!leavingQueue.empty()) leavingQueue.pop();
    nextIndex = 0;
    elapsedTime = 0;
    totalSpawned = 0;
    totalLeft = 0;

    //ˢ�·���������
    ScoreManager::Instance()->RestartTimer();
}