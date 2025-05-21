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

    #pragma region UpdatePendingQueue
    //�ѵ���ʱ���ѵ��������ɴ����߳���
    if (nextIndex < tasks.size() && tasks[nextIndex].spawnTime <= elapsedTime)
    {
        const VehicleSpawnTask& _task = tasks[nextIndex];

        //��Task��Vehicle����һһ��Ӧ�����ӵ���֮һ�֣�
        Vehicle* v = new Vehicle();
        v->SetPosition((int)_task.spawnPosition.x, (int)_task.spawnPosition.y);
        v->SetElectricity(_task.initialElectricity);
        v->SetTargetElectricity(_task.requiredElectricity);
        v->SetLeaveTime(_task.leaveTime);
        v->vehicleNo = _task.VehicleTaskNo;

		//���ó����ĳ�ʼ�볡����
        Vector2 _spawnDirection;
        _spawnDirection.x = (_task.targetPosition.x - _task.spawnPosition.x);
        _spawnDirection.y = (_task.targetPosition.y - _task.spawnPosition.y);
        //���ݷ������������ʴ�С�������볡�ٶ�����
        v->SetVelocity(_spawnDirection.Normalized() * v->GetSpeed());

		//��δ��ʽ������ȣ�ֻ�е���Ŀ��λ�ú������ʽ����
        v->isOnline = false;
        v->arriveTime = _task.spawnTime;

        //����ȴ����ߵĶ�����
        pendingQueue.push(v);
        nextIndex++;
    }
    #pragma endregion
    
    #pragma region UpdateComingQueueFromPendingQueue
    //���pendingQueue����Щ��������ʽ����
    size_t _pendingqueueSize = pendingQueue.size();
    for (size_t _i = 0; _i < _pendingqueueSize; _i++)
    {
        Vehicle* _v = pendingQueue.front();
        //����Ϊ�����ƶ�����ֹ�����������
        _v->isMoving = true;
        pendingQueue.pop();
        if (_v->arriveTime <= elapsedTime)
        {
            //����ʱ�����㣬��������ϵͳ
            _v->isOnline = true;
            ChargeableManager::Instance()->AddExternalChargeable(ChargeableType::Vehicle, _v);

            //��������ǰ��Ŀ��λ�õĳ�������
            comingQueue.push(_v);
        }
        //�������㣬���ٽ������ض�β
        else
            pendingQueue.push(_v);
    }
    #pragma endregion
    
    #pragma region UpdateWorkingQueueFromComingQueue
    size_t _comingQueueSize = comingQueue.size();
    for (size_t _i = 0; _i < _comingQueueSize; _i++)
    {
		//��ǰ��Ŀ��λ�õĶ�����ȡ������
        Vehicle* _v = comingQueue.front();
        comingQueue.pop();
        //��ֹ���ɳ���ʱ������
        _v->isTouchingMapBorder = false;
        int _index = 0;
        for (; _index < tasks.size(); _index++)
        {
            //��_indexʹ��Task��Vehicle��Ӧ��ʱ������
            if (tasks[_index].VehicleTaskNo == _v->vehicleNo)
                break;
        }
        //�ж��ǲ��ǵ���Ŀ��λ�ã���һ��TILE_SIZE����������ж�
        if ((int)_v->GetPosition().x <= (int)tasks[_index].targetPosition.x + TILE_SIZE / 2
            && (int)_v->GetPosition().x >= (int)tasks[_index].targetPosition.x - TILE_SIZE / 2
            && (int)_v->GetPosition().y <= (int)tasks[_index].targetPosition.y + TILE_SIZE / 2
            && (int)_v->GetPosition().y >= (int)tasks[_index].targetPosition.y - TILE_SIZE / 2)
        {
			//���侲ֹǶ��Ŀ��λ��
            _v->SetVelocity({ 0,0 });
            _v->SetPosition((int)tasks[_index].targetPosition.x, (int)tasks[_index].targetPosition.y);
            //���빤������
            workingQueue.push(_v);
        }
        //��������ȥ
        else
            comingQueue.push(_v);
    }
    #pragma endregion

    #pragma region UpdateLeavingQueueFromWorkingQueue
    size_t _workingQueueSize = workingQueue.size();    
    if (_workingQueueSize) for (size_t i = 0; i < _workingQueueSize; i++)
    {
		//ȡ�����������еĳ���
        Vehicle* _v = workingQueue.front();
        workingQueue.pop();
        //����Ϊ�����ƶ��У��û����������
        _v->isMoving = false;
        int _index = 0;
        for (; _index < tasks.size(); _index++)
        {
            //��_indexʹ��Task��Vehicle��Ӧ��ʱ������
            if (tasks[_index].VehicleTaskNo == _v->vehicleNo)
                break;
        }
        //�����������������뿪ʱ��ʱ�����뿪����
        if (!_v->NeedElectricity() || tasks[_index].leaveTime <= elapsedTime)
        {
            //�����뿪ʱ����
            Vector2 endDirection = { 0,0 };
            endDirection.x = (tasks[_index].leavePosition.x - tasks[_index].targetPosition.x);
            endDirection.y = (tasks[_index].leavePosition.y - tasks[_index].targetPosition.y);
            //���뿪��λ�ñ�ȥ
            _v->SetVelocity(endDirection.Normalized() * _v->GetSpeed());
            leavingQueue.push(_v);
        }
        else
            workingQueue.push(_v);
    }
    #pragma endregion
    
    #pragma region UpdateLeavingQueue
    size_t _leavingQueueSize = leavingQueue.size();  
    for (size_t i = 0; i < _leavingQueueSize; i++)
    {
		//ȡ���뿪�����еĳ���
        Vehicle* v = leavingQueue.front();
        leavingQueue.pop();
        //����Ϊ�����ƶ�����ֹǱ������
        v->isMoving = true;
        int _index = 0;
        for (; _index < tasks.size(); _index++)
        {
            //��_indexʹ��Task��Vehicle��Ӧ��ʱ������
            if (tasks[_index].VehicleTaskNo == v->vehicleNo)
                break;
        }
        //���˱߽�λ�ú����
        if (v->isTouchingMapBorder == true)
            v->Invalidate();
        else
            leavingQueue.push(v);
    }
    #pragma endregion
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

    //ˢ�·���������
    ScoreManager::Instance()->RestartTimer();
}