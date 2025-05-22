#include "../../../Header/Manager/Concrete/SpawnManager.h"
#include <set>
#include <iostream>
#include<random>
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ScoreManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

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
    ChangeVehicleLevel(_level);
    ChangeRobotLevel(_level);
}

SpawnManager::ScaleLevel SpawnManager::GetCurrentLevel() const
{
    return currentScaleLevel;
}

size_t SpawnManager::GetHitVehicleNum() const
{
    return hitVehicleNum;
}

size_t SpawnManager::GetMissVehicleNum() const
{
    return missVehicleNum;
}

void SpawnManager::RefreshVehicleTasks()
{
    //重置载具生成相关数据
    tasks.clear();
    while (!pendingQueue.empty()) pendingQueue.pop();
    while (!comingQueue.empty()) comingQueue.pop();
    while (!workingQueue.empty()) workingQueue.pop();
    while (!leavingQueue.empty()) leavingQueue.pop();
    nextIndex = 0;
    elapsedTime = 0;

    //刷新时间与计数数据
    ScoreManager::Instance()->RestartTimer();
    hitVehicleNum = 0;
    missVehicleNum = 0;
}

void SpawnManager::UpdateVehicleSpawn(double _delta)
{
    //累加仿真时间
    elapsedTime += _delta;

    #pragma region UpdatePendingQueue
    //把到达时间已到的任务变成待上线车辆
    if (nextIndex < tasks.size() && tasks[nextIndex].spawnTime <= elapsedTime)
    {
        const VehicleSpawnTask& _task = tasks[nextIndex];

        //将Task和Vehicle对象一一对应（罗子的神之一手）
        Vehicle* _v = new Vehicle();
        _v->SetPosition((int)_task.spawnPosition.x, (int)_task.spawnPosition.y);
        _v->SetElectricity(_task.spawnElectricity);
        _v->SetTargetElectricity(_task.leaveElectricity);
        _v->SetLeaveTime(_task.leaveTime);
        _v->vehicleFlag = _task.vehicleTaskNo;

		//设置车辆的初始入场方向
        Vector2 _spawnDirection;
        _spawnDirection.x = (_task.targetPosition.x - _task.spawnPosition.x);
        _spawnDirection.y = (_task.targetPosition.y - _task.spawnPosition.y);
        //根据方向向量和速率大小，设置入场速度向量
        _v->SetVelocity(_spawnDirection.Normalized() * _v->GetSpeed());

		//还未正式进入调度，只有到达目标位置后才能正式上线
        _v->isOnline = false;
        _v->arriveTime = _task.spawnTime;

        //放入等待上线的队列中
        pendingQueue.push(_v);
        nextIndex++;
    }
    #pragma endregion
    
    #pragma region UpdateComingQueueFromPendingQueue
    //检查pendingQueue中哪些车可以正式上线
    size_t _pendingqueueSize = pendingQueue.size();
    for (size_t _i = 0; _i < _pendingqueueSize; _i++)
    {
        Vehicle* _v = pendingQueue.front();
        //设置为正在移动，防止机器人来充电
        _v->isMoving = true;
        pendingQueue.pop();
        if (_v->arriveTime <= elapsedTime)
        {
            //到达时间满足，则加入调度系统
            _v->isOnline = true;
            ChargeableManager::Instance()->AddExternalChargeable(ChargeableType::Vehicle, _v);

            //加入正在前往目标位置的车辆队列
            comingQueue.push(_v);
        }
        //若不满足，则再将其塞回队尾
        else
            pendingQueue.push(_v);
    }
    #pragma endregion
    
    #pragma region UpdateWorkingQueueFromComingQueue
    size_t _comingQueueSize = comingQueue.size();
    for (size_t _i = 0; _i < _comingQueueSize; _i++)
    {
		//从前往目标位置的队列中取出车辆
        Vehicle* _v = comingQueue.front();
        comingQueue.pop();
        //防止生成车辆时的误判
        _v->isTouchingMapBorder = false;
        int _index = 0;
        for (; _index < tasks.size(); _index++)
        {
            //当_index使得Task和Vehicle对应的时候跳出
            if (tasks[_index].vehicleTaskNo == _v->vehicleFlag)
                break;
        }
        //判断是不是到了目标位置，以一个TILE_SIZE的区间进行判断
        if ((int)_v->GetPosition().x <= (int)tasks[_index].targetPosition.x + TILE_SIZE / 2
            && (int)_v->GetPosition().x >= (int)tasks[_index].targetPosition.x - TILE_SIZE / 2
            && (int)_v->GetPosition().y <= (int)tasks[_index].targetPosition.y + TILE_SIZE / 2
            && (int)_v->GetPosition().y >= (int)tasks[_index].targetPosition.y - TILE_SIZE / 2)
        {
			//将其静止嵌入目标位置
            _v->SetVelocity({ 0,0 });
            _v->SetPosition((int)tasks[_index].targetPosition.x, (int)tasks[_index].targetPosition.y);
            //加入工作队列
            workingQueue.push(_v);
        }
        //否则塞回去
        else
            comingQueue.push(_v);
    }
    #pragma endregion

    #pragma region UpdateLeavingQueueFromWorkingQueue
    size_t _workingQueueSize = workingQueue.size();    
    if (_workingQueueSize) for (size_t i = 0; i < _workingQueueSize; i++)
    {
		//取出工作队列中的车辆
        Vehicle* _v = workingQueue.front();
        workingQueue.pop();
        //设置为不在移动中，让机器人来充电
        _v->isMoving = false;
        int _index = 0;
        for (; _index < tasks.size(); _index++)
        {
            //当_index使得Task和Vehicle对应的时候跳出
            if (tasks[_index].vehicleTaskNo == _v->vehicleFlag)
                break;
        }
        //当到达电量需求或者离开时间时进入离开队列
        bool _flagHit = !_v->NeedElectricity();
        bool _flagMiss = tasks[_index].leaveTime <= elapsedTime;
        if (_flagHit || _flagMiss)
        {
            //设置离开时方向
            Vector2 endDirection = { 0,0 };
            endDirection.x = (tasks[_index].leavePosition.x - tasks[_index].targetPosition.x);
            endDirection.y = (tasks[_index].leavePosition.y - tasks[_index].targetPosition.y);
            //向离开的位置奔去
            _v->SetVelocity(endDirection.Normalized() * _v->GetSpeed());
            leavingQueue.push(_v);

			//增加计数，并播放音效
            if (_flagHit)
            {
                hitVehicleNum++;
                AudioManager::Instance()->PlaySFX(SoundResID::HitVehicle);
            }
            else if (_flagMiss)
            {
                missVehicleNum++;
                AudioManager::Instance()->PlaySFX(SoundResID::MissVehicle);
            }
        }
        else
            workingQueue.push(_v);
    }
    #pragma endregion
    
    #pragma region UpdateLeavingQueue
    size_t _leavingQueueSize = leavingQueue.size();  
    for (size_t i = 0; i < _leavingQueueSize; i++)
    {
		//取出离开队列中的车辆
        Vehicle* _v = leavingQueue.front();
        leavingQueue.pop();
        //设置为正在移动，防止潜在问题
        _v->isMoving = true;
        int _index = 0;
        for (; _index < tasks.size(); _index++)
        {
            //当_index使得Task和Vehicle对应的时候跳出
            if (tasks[_index].vehicleTaskNo == _v->vehicleFlag)
                break;
        }
        //到了边界位置后清除
        if (_v->isTouchingMapBorder == true)
            _v->Invalidate();
        else
            leavingQueue.push(_v);
    }
    #pragma endregion
}

void SpawnManager::ChangeVehicleLevel(ScaleLevel _level)
{
    //重置当前场景中的车辆相关变量
    RefreshVehicleTasks();

    #pragma region VehicleNum
    size_t _vehicleNum = 0;
    switch (_level)
    {
    case SpawnManager::ScaleLevel::Small:
        _vehicleNum = vehicleNumLevelSmall;
        break;
    case SpawnManager::ScaleLevel::Medium:
        _vehicleNum = vehicleNumLevelMedium;
        break;
    case SpawnManager::ScaleLevel::Large:
        _vehicleNum = vehicleNumLevelLarge;
        break;
    default:
        break;
    }
    #pragma endregion

    size_t _mapTilesX = SceneManager::Instance()->map.GetWidthTileNum();
    size_t _mapTilesY = SceneManager::Instance()->map.GetHeightTileNum();

    //随机创建所有载具生成任务
    for (size_t _i = 0; _i < _vehicleNum; _i++)
    {
		//创建一个载具生成任务
        VehicleSpawnTask _task;
		//载具生成任务的唯一标识符
        _task.vehicleTaskNo = _i;
        
        #pragma region RandomSpawnAndLeaveTime
        _task.spawnTime = (double)(rand() % spawnTimeUpper + _i * 1);
        _task.leaveTime = (double)(_task.spawnTime + leaveTimeSpan);
        #pragma endregion

        #pragma region RandomElectricityDemand
        //初始电量及任务电量需求
        _task.spawnElectricity = (double)(rand() % spawnElectricityUpper + 1);
        _task.leaveElectricity = (double)(leaveElectricityLower + rand() % (100 - leaveElectricityLower) + 1);
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

        tasks.push_back(_task);
    }
}

void SpawnManager::ChangeRobotLevel(ScaleLevel _level)
{
    //根据难度等级决定要生成的机器数量
    size_t _robotNum = 0;
    switch (_level)
    {
    case SpawnManager::ScaleLevel::Small:
        _robotNum = robotNumLevelSmall;
        break;
    case SpawnManager::ScaleLevel::Medium:
        _robotNum = robotNumLevelMedium;
        break;
    case SpawnManager::ScaleLevel::Large:
        _robotNum = robotNumLevelLarge;
        break;
    default:
        break;
    }

    for (size_t _i = 0; _i < _robotNum; _i++)
        ChargeableManager::Instance()->SpawnChargeableAt(ChargeableType::Robot, { 0,0 });
}