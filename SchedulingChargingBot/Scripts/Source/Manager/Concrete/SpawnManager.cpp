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

    //清空
    ChargeableManager::Instance()->ClearAll();

    //重新加载
    LoadVehicleLevel(_level);
    LoadRobotLevel(_level);
}

SpawnManager::ScaleLevel SpawnManager::GetCurrentLevel() const
{
    return currentScaleLevel;
}

void SpawnManager::UpdateVehicleSpawn(double _delta)
{
    //累加仿真时间
    elapsedTime += _delta;

    //把“到达时间已到”的任务变成待上线车辆
    while (nextIndex < tasks.size() && tasks[nextIndex].spawnTime <= elapsedTime)
    {
        const auto& t = tasks[nextIndex];

        Vehicle* v = new Vehicle();
        v->SetPosition((int)t.spawnPosition.x, (int)t.spawnPosition.y);
        v->SetElectricity(t.initialElectricity);
        v->SetTargetElectricity(t.requiredElectricity);
        v->SetLeaveTime(t.leaveTime);
        v->VehicleNo = t.VehicleTaskNo;//将task和vehicle对象一一对应。（罗子的神之一手）

        Vector2 initDirection;
        initDirection.x = (t.targetPosition.x - t.spawnPosition.x);
        initDirection.y = (t.targetPosition.y - t.spawnPosition.y);//设置入场时方向。

        v->SetVelocity(initDirection.Normalized() * v->GetSpeed());

        v->isOnline = false;     // 刚到达，还未正式进入调度
        v->arriveTime = t.spawnTime;

        pendingQueue.push(v);    // 放入等待上线队列
        ++nextIndex;
    }
    
    // 检查 pendingQueue 中哪些车可以正式上线
    size_t pendingqueueSize = pendingQueue.size();
    if (pendingqueueSize) for (size_t i = 0; i < pendingqueueSize; ++i) {
        Vehicle* v = pendingQueue.front();
        v->isMoving = true;//设置为正在移动，防止机器人来充电。
        pendingQueue.pop();
        if (v->arriveTime <= elapsedTime)
        {
            // 到达时间满足 → 加入调度系统
            v->isOnline = true;
            //v->ChangeState("Charged");
            ChargeableManager::Instance()->AddExternalChargeable(ChargeableType::Vehicle, v);

            printf("[ARRIVE] Vehicle at (%.0f, %.0f), elec=%.0f%%, needs=%.0f%%, leave=%.0fs\n",
                v->GetPosition().x, v->GetPosition().y,
                v->GetElectricity(),    // 使用 %.0f 格式化为无小数位的double
                v->GetTargetElectricity(),
                v->GetLeaveTime());
            comingQueue.push(v);//加入正在来的车辆队列
        }
        else pendingQueue.push(v);//如果不满足，则再从队尾加入队列
    }
    
    size_t comingQueueSize = comingQueue.size();
    if (comingQueueSize) for (size_t i = 0; i < comingQueueSize; ++i) {
        Vehicle* v = comingQueue.front();
        comingQueue.pop();
        //std::cout << "comingQueueSize：" << comingQueueSize << std::endl;
        v->IsTouchingMapBorder = false;//防止生成车辆时的误判
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//当index使得task和vehicle对应的时候跳出。
        }
        //std::cout << "v->GetPosition().x" << v->GetPosition().x << std::endl << "tasks[index].position.x" << tasks[index].position.x << std::endl;
        if ((int)v->GetPosition().x == (int)tasks[index].targetPosition.x || (int)v->GetPosition().y == (int)tasks[index].targetPosition.y) {//判断是不是到了目标位置，这里可以更完善，但现在先偷个懒（
            v->SetVelocity({ 0,0 });
            v->SetPosition((int)tasks[index].targetPosition.x, (int)tasks[index].targetPosition.y);
            workingQueue.push(v);
        }
        else comingQueue.push(v);
    }

    size_t workingQueueSize = workingQueue.size();    
    if (workingQueueSize) for (size_t i = 0; i < workingQueueSize; ++i) {
        Vehicle* v = workingQueue.front();
        v->isMoving = false;//设置为不在移动中，让机器人来充电。
        workingQueue.pop();
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//当index使得task和vehicle对应的时候跳出。
        }
        if (!v->NeedElectricity() || tasks[index].leaveTime <= elapsedTime) {//当到达电量需求或者离开时间时进入离开队列。
            Vector2 endDirection = { 0 , 0 };
            endDirection.x = (tasks[index].leavePosition.x - tasks[index].targetPosition.x);
            endDirection.y = (tasks[index].leavePosition.y - tasks[index].targetPosition.y);
            //上面是设置离开时方向。
            v->SetVelocity(endDirection.Normalized() * v->GetSpeed());//向离开的位置奔去
            //std::cout << "向夜晚奔去" << std::endl;
            leavingQueue.push(v);
        }
        else workingQueue.push(v);
    }
    
    size_t leavingQueueSize = leavingQueue.size();  
    if (leavingQueueSize) for (size_t i = 0; i < leavingQueueSize; ++i)
    {
        Vehicle* v = leavingQueue.front();
        //设置为正在移动，防止潜在问题
        v->isMoving = true;
        leavingQueue.pop();
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            //当index使得task和vehicle对应的时候跳出
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
    //重置当前场景中的车辆相关变量
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

    //随机创建所有载具生成任务
    for (int _i = 0; _i < _vehicleCount; _i++)
    {
		//创建一个载具生成任务
        VehicleSpawnTask _task;
		//载具生成任务的唯一标识符
        _task.VehicleTaskNo = _i;
        
        #pragma region RandomArriveTime
        _task.spawnTime = rand() % 5 + _i * 2;
        #pragma endregion

        #pragma region RandomPosition
        size_t _targetTileX = 0, _targetTileY = 0;
        size_t _spawnTileX = 0, _spawnTileY = 0;
        size_t _leaveTileX = 0, _leaveTileY = 0;

		//让目标位置处于整个地图向内凹陷2个瓦片单位的区域内
        _targetTileX = rand() % (_mapTilesX - 4) + 2;
        _targetTileY = rand() % (_mapTilesY - 4) + 2;

		//载具在边缘出现和离开的位置，随机挑一个边：0上，1下，2左，3右
        size_t _edge = rand() % 4;
        switch (_edge)
        {
        case 0: //上边
            _spawnTileX = rand() % _mapTilesX;
            _spawnTileY = 0;
            _leaveTileX = rand() % _mapTilesX;
            _leaveTileY = 0;
            break;
        case 1: //下边
            _spawnTileX = rand() % _mapTilesX;
            _spawnTileY = _mapTilesY - 1;
            _leaveTileX = rand() % _mapTilesX;
            _leaveTileY = _mapTilesY - 1;
            break;
        case 2: //左边
            _spawnTileX = 0;
            _spawnTileY = rand() % _mapTilesY;
            _leaveTileX = 0;
            _leaveTileY = rand() % _mapTilesY;
            break;
        case 3: //右边
            _spawnTileX = _mapTilesX - 1;
            _spawnTileY = rand() % _mapTilesY;
            _leaveTileX = _mapTilesX - 1;
            _leaveTileY = rand() % _mapTilesY;
            break;
        }
        
        //车辆任务生成的坐标
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
        //初始电量及任务电量需求
        _task.initialElectricity = rand() % 40 + 20;
        _task.requiredElectricity = 80 + rand() % 21;
        _task.leaveTime = _task.spawnTime + 60 + rand() % 61;
        #pragma endregion

        tasks.push_back(_task);
    }
}

void SpawnManager::LoadRobotLevel(ScaleLevel _level)
{
    //根据难度等级决定要生成的机器数量
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
    //清空载具生成相关数据
    tasks.clear();
    while (!pendingQueue.empty()) pendingQueue.pop();
    while (!comingQueue.empty()) comingQueue.pop();
    while (!workingQueue.empty()) workingQueue.pop();
    while (!leavingQueue.empty()) leavingQueue.pop();
    nextIndex = 0;
    elapsedTime = 0;
    totalSpawned = 0;
    totalLeft = 0;

    //刷新分数管理器
    ScoreManager::Instance()->RestartTimer();
}