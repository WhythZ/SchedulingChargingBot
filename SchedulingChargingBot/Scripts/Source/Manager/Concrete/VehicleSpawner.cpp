#include "../../../Header/Manager/Concrete/VehicleSpawner.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Tilemap/Tile.h"

#include <cstdlib>
#include <set>
#include <cstdio>
#include <iostream>

// 载入不同场景等级对应的车辆生成任务
void VehicleSpawner::LoadScenario(int _level)
{
    tasks.clear();       //清空旧任务
    nextIndex = 0;       //重置索引
    elapsedTime = 0;     //重置仿真时间

    //清空等待队列
    while (!pendingQueue.empty()) pendingQueue.pop();

    //根据等级决定要生成的车辆数量
    int _vehicleCount = (_level == 0) ? 5 : (_level == 1) ? 10 : 20;

    //动态获取地图的瓦片数量（行列数）
    int _mapTilesX = SceneManager::Instance()->mapRect.w / TILE_SIZE;
    int _mapTilesY = SceneManager::Instance()->mapRect.h / TILE_SIZE;

    //记录已用瓦片，避免重复生成
    std::set<std::pair<int, int>> occupiedTiles;

    for (int _i = 0; _i < _vehicleCount; _i++)
    {
        VehicleSpawnTask task;
        //标记车辆编号，用于辨识每一辆车
        task.VehicleTaskNo = _i;

        //设置车辆到达时间：逐车推迟并加随机扰动，避免集中生成
        task.spawnTime = rand() % 10 + _i * 3;

        int _tileX, _tileY;
        int _tileInitX, _tileInitY;
        int _tileEndX, _tileEndY;

        //随机选择未占用的瓦片，避开地图边缘2格；初始位置和最终位置都是在地图边缘
        do
        {
            _tileX = rand() % (_mapTilesX - 4) + 2;
            _tileY = rand() % (_mapTilesY - 4) + 2;

            //起点生成逻辑
            _tileInitX = rand() % _mapTilesX;
            _tileInitY = rand() % _mapTilesY;

            //选择是靠X边界还是Y边界
            int a = rand() % 2;

            _tileInitX = a ? (rand() % 2) * (_mapTilesX - 1) : _tileInitX; //X=0或X=max
            _tileInitY = a ? _tileInitY : (rand() % 2) * (_mapTilesY - 1); //Y=0或Y=max

            //终点生成逻辑
            _tileEndX = rand() % _mapTilesX;
            _tileEndY = rand() % _mapTilesY;

            //选择是靠X边界还是Y边界
            int b = rand() % 2;

            _tileEndX = b ? (rand() % 2) * (_mapTilesX - 1) : _tileEndX;
            _tileEndY = b ? _tileEndY : (rand() % 2) * (_mapTilesY - 1);

        } while (occupiedTiles.count({ _tileX, _tileY }) && occupiedTiles.count({ _tileInitX, _tileInitY }) && occupiedTiles.count({ _tileEndX, _tileEndY }));  // 确保不重复

        //记录这些瓦片已被使用
        occupiedTiles.insert({ _tileX, _tileY });
        occupiedTiles.insert({ _tileInitX, _tileInitY });
        occupiedTiles.insert({ _tileEndX, _tileEndY });

        //设置车辆的实际位置、初始位置和离开位置（转换为像素坐标，居中于瓦片）
        task.position = {
            static_cast<double>(_tileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(_tileY * TILE_SIZE + TILE_SIZE / 2)
        };

        task.position_spawn = {
            static_cast<double>(_tileInitX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(_tileInitY * TILE_SIZE + TILE_SIZE / 2)
        };

        task.position_leave = {
            static_cast<double>(_tileEndX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(_tileEndY * TILE_SIZE + TILE_SIZE / 2)
        };

        //初始电量为 20%~59%
        task.initialElectricity = rand() % 40 + 20;

        //离开时要求的电量为 80%~100%
        task.requiredElectricity = 80 + rand() % 21;

        //离开时间为到达时间后 60~120 秒
        task.leaveTime = task.spawnTime + 60 + rand() % 61;

        //加入任务列表
        tasks.push_back(task);
    }
}

//在每一帧更新时调用，处理车辆到达与上线
void VehicleSpawner::OnUpdate(double delta)
{
    elapsedTime += delta;  // 累加仿真时间

    // 把“到达时间已到”的任务变成待上线车辆
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
        initDirection.y = (t.position.y - t.position_spawn.y);//设置入场时方向

        v->SetVelocity(initDirection.Normalized() * 250);

        //刚到达，还未正式进入调度
        v->isOnline = false;
        v->arriveTime = t.spawnTime;

        //放入等待上线队列
        pendingQueue.push(v);
        ++nextIndex;
    }
    // 检查pendingQueue中哪些车可以正式上线
    size_t pendingqueueSize = pendingQueue.size();
    if (pendingqueueSize) for (size_t i = 0; i < pendingqueueSize; ++i)
    {
        Vehicle* v = pendingQueue.front();
        //设置为正在移动，防止机器人来充电
        v->isMoving = true;
        pendingQueue.pop();
        if (v->arriveTime <= elapsedTime) {
            // 到达时间满足 → 加入调度系统
            v->isOnline = true;
            //v->ChangeState("Charged");
            ChargeableManager::Instance()->AddChargeable(v);

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
            v->SetPosition((int)tasks[index].position.x ,(int)tasks[index].position.y);
            workingQueue.push(v);
        }
        else comingQueue.push(v);
    }
    size_t workingQueueSize = workingQueue.size();
    if (workingQueueSize) for (size_t i = 0; i < workingQueueSize; ++i) {
        Vehicle* v = workingQueue.front();
        v->isMoving = false;//设置为不在移动中，让机器人来充电。
        workingQueue.pop();
        std::cout << "workingQueueSize：" << workingQueueSize << std::endl;
        std::cout << "vehicle current electricity:" << v->GetCurrentElectricity() << std::endl;
        std::cout << "vehicle target electricity:" << v->GetTargetElectricity() << std::endl;
        std::cout << "vehicle isCharged :" << v->isCharged << std::endl;
        std::cout << "vehicle isTargeted :" << v->isTargeted << std::endl;
        std::cout << "vehicle isbusy :" << v->IsBusy() << std::endl;
        std::cout << "vehicle isMoving :" << v->rtisMoving() << std::endl;
        std::cout << "vehicle charger :" << v->charger << std::endl;
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//当index使得task和vehicle对应的时候跳出。
        }
        if (!v->NeedElectricity() || tasks[index].leaveTime <= elapsedTime) {//当到达电量需求或者离开时间时进入离开队列。
            Vector2 endDirection = { 0 , 0 };
            //设置离开时方向
            endDirection.x = (tasks[index].position_leave.x - tasks[index].position.x);
            endDirection.y = (tasks[index].position_leave.y - tasks[index].position.y);
            //向离开的位置奔去
            v->SetVelocity(endDirection.Normalized() * v->GetSpeed());
            //std::cout << "向夜晚奔去" << std::endl;
            leavingQueue.push(v);
        }
        else workingQueue.push(v);
    }
    size_t leavingQueueSize = leavingQueue.size();
    if (leavingQueueSize) for (size_t i = 0; i < leavingQueueSize; ++i) {
        //std::cout << "leavingQueueSize:" << leavingQueueSize << std::endl;
        Vehicle* v = leavingQueue.front();
        v->isMoving = true;//设置为正在移动，防止潜在问题。
        leavingQueue.pop();
        int index = 0;
        for (; index < tasks.size(); ++index)
        {
            if (tasks[index].VehicleTaskNo == v->VehicleNo)
                break;//当index使得task和vehicle对应的时候跳出。
        }
        //std::cout << "触及边界？" << v->IsTouchingMapBorder << std::endl;
        if (v->IsTouchingMapBorder == true) {
            v->Invalidate();//设置为invalid，等待manager清除。
        }
        else leavingQueue.push(v);
    }
}