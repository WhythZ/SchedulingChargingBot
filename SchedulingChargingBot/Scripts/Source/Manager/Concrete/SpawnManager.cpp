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
        v->SetPosition((int)t.position_spawn.x, (int)t.position_spawn.y);
        v->SetElectricity(t.initialElectricity);
        v->SetTargetElectricity(t.requiredElectricity);
        v->SetLeaveTime(t.leaveTime);
        v->VehicleNo = t.VehicleTaskNo;//将task和vehicle对象一一对应。（罗子的神之一手）

        Vector2 initDirection;
        initDirection.x = (t.position.x - t.position_spawn.x);
        initDirection.y = (t.position.y - t.position_spawn.y);//设置入场时方向。

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
        if ((int)v->GetPosition().x == (int)tasks[index].position.x || (int)v->GetPosition().y == (int)tasks[index].position.y) {//判断是不是到了目标位置，这里可以更完善，但现在先偷个懒（
            v->SetVelocity({ 0,0 });
            v->SetPosition((int)tasks[index].position.x, (int)tasks[index].position.y);
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
            endDirection.x = (tasks[index].position_leave.x - tasks[index].position.x);
            endDirection.y = (tasks[index].position_leave.y - tasks[index].position.y);
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

    int _mapTilesX = SceneManager::Instance()->map.GetWidthTileNum();
    int _mapTilesY = SceneManager::Instance()->map.GetHeightTileNum();

    std::set<std::pair<int, int>> _occupiedTiles;

    for (int _i = 0; _i < _vehicleCount; _i++)
    {
        VehicleSpawnTask _task;
        _task.VehicleTaskNo = _i;
        _task.spawnTime = rand() % 10 + _i * 3;

        int tileX, tileY;
        int tile_initX, tile_initY;
        int tile_endX, tile_endY;

        //设定最大尝试次数，防止死循环
        const int MAX_TRIES = 1000;
        int tries = 0;

        do
        {
            tileX = rand() % (_mapTilesX - 8) + 4;
            tileY = rand() % (_mapTilesY - 8) + 4;

            tile_initX = rand() % _mapTilesX;
            tile_initY = rand() % _mapTilesY;

            int a = rand() % 2;
            tile_initX = a ? (rand() % 2) * (_mapTilesX - 1) : tile_initX;
            tile_initY = a ? tile_initY : (rand() % 2) * (_mapTilesY - 1);

            tile_endX = rand() % _mapTilesX;
            tile_endY = rand() % _mapTilesY;

            int b = rand() % 2;
            tile_endX = b ? (rand() % 2) * (_mapTilesX - 1) : tile_endX;
            tile_endY = b ? tile_endY : (rand() % 2) * (_mapTilesY - 1);

            tries++;
            if (tries > MAX_TRIES) {
                std::cerr << "Warning: Could not find unoccupied tiles after " << MAX_TRIES << " tries\n";
                break;
            }

        } while (_occupiedTiles.count({ tileX, tileY })
            || _occupiedTiles.count({ tile_initX, tile_initY })
            || _occupiedTiles.count({ tile_endX, tile_endY }));

        _occupiedTiles.insert({ tileX, tileY });
        _occupiedTiles.insert({ tile_initX, tile_initY });
        _occupiedTiles.insert({ tile_endX, tile_endY });

        //车辆任务生成的坐标
        _task.position =
        {
            static_cast<double>(tileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tileY * TILE_SIZE + TILE_SIZE / 2)
        };
        _task.position_spawn =
        {
            static_cast<double>(tile_initX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tile_initY * TILE_SIZE + TILE_SIZE / 2)
        };
        _task.position_leave =
        {
            static_cast<double>(tile_endX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tile_endY * TILE_SIZE + TILE_SIZE / 2)
        };

        //初始电量及任务电量需求
        _task.initialElectricity = rand() % 40 + 20;
        _task.requiredElectricity = 80 + rand() % 21;
        _task.leaveTime = _task.spawnTime + 60 + rand() % 61;

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