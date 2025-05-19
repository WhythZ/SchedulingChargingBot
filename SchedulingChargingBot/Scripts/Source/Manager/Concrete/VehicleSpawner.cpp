﻿#include "../../../Header/Manager/Concrete/VehicleSpawner.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Tilemap/Tile.h"

#include <cstdlib>
#include <set>
#include <cstdio>
#include <iostream>

// 载入不同场景等级对应的车辆生成任务
void VehicleSpawner::LoadScenario(int level)
{
    tasks.clear();       // 清空旧任务
    nextIndex = 0;       // 重置索引
    elapsedTime = 0;     // 重置仿真时间

    while (!pendingQueue.empty()) pendingQueue.pop(); // 清空等待队列

    // 根据难度等级决定要生成的车辆数量
    int vehicleCount = (level == 0) ? 5 : (level == 1) ? 10 : 20;

    // 动态获取地图的瓦片数量（行列数）
    int mapTilesX = SceneManager::Instance()->mapRect.w / TILE_SIZE;
    int mapTilesY = SceneManager::Instance()->mapRect.h / TILE_SIZE;

    std::set<std::pair<int, int>> occupiedTiles;  // 记录已用瓦片，避免重复生成

    for (int i = 0; i < vehicleCount; ++i) {
        VehicleSpawnTask task;
        task.VehicleTaskNo = i;//标记车辆编号，用于辨识每一辆车。

        // 设置车辆到达时间：逐车推迟并加随机扰动，避免集中生成
        task.spawnTime = rand() % 10 + i * 3;

        int tileX, tileY;
        int tile_initX, tile_initY;
        int tile_endX, tile_endY;
        // 随机选择未占用的瓦片，避开地图边缘2格
        // 初始位置和最终位置都是在地图边缘
        do {
            tileX = rand() % (mapTilesX - 8) + 4;
            tileY = rand() % (mapTilesY - 8) + 4;

            // 起点生成逻辑
            tile_initX = rand() % mapTilesX;
            tile_initY = rand() % mapTilesY;

            int a = rand() % 2; // 选择是靠 X 边界 还是 Y 边界

            tile_initX = a ? (rand() % 2) * (mapTilesX - 1) : tile_initX; // X=0 或 X=max
            tile_initY = a ? tile_initY : (rand() % 2) * (mapTilesY - 1); // Y=0 或 Y=max

            // 终点生成逻辑
            tile_endX = rand() % mapTilesX;
            tile_endY = rand() % mapTilesY;

            int b = rand() % 2; // 选择是靠 X 边界 还是 Y 边界

            tile_endX = b ? (rand() % 2) * (mapTilesX - 1) : tile_endX;
            tile_endY = b ? tile_endY : (rand() % 2) * (mapTilesY - 1);

        } while (occupiedTiles.count({ tileX, tileY }) && occupiedTiles.count({ tile_initX, tile_initY }) && occupiedTiles.count({ tile_endX, tile_endY }));  // 确保不重复

        occupiedTiles.insert({ tileX, tileY });  // 记录这些瓦片已被使用
        occupiedTiles.insert({ tile_initX, tile_initY });
        occupiedTiles.insert({ tile_endX, tile_endY });

        // 设置车辆的实际位置、初始位置和离开位置（转换为像素坐标，居中于瓦片）
        task.position = {
            static_cast<double>(tileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tileY * TILE_SIZE + TILE_SIZE / 2)
        };

        task.position_spawn = {
            static_cast<double>(tile_initX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tile_initY * TILE_SIZE + TILE_SIZE / 2)
        };

        task.position_leave = {
            static_cast<double>(tile_endX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tile_endY * TILE_SIZE + TILE_SIZE / 2)
        };

        // 初始电量为 20%~59%
        task.initialElectricity = rand() % 40 + 20;

        // 离开时要求的电量为 80%~100%
        task.requiredElectricity = 80 + rand() % 21;

        // 离开时间为到达时间后 60~120 秒
        task.leaveTime = task.spawnTime + 60 + rand() % 61;

        tasks.push_back(task);  // 加入任务列表
    }
}

// 在每一帧更新时调用，处理车辆到达与上线
// 这里使用了四个队列，在VehicleSpawner.h文件里有说明。或许我该写个#pragma region？先这样吧。
void VehicleSpawner::OnUpdate(double delta)
{
    elapsedTime += delta;  // 累加仿真时间

    // 把“到达时间已到”的任务变成待上线车辆
    while (nextIndex < tasks.size() && tasks[nextIndex].spawnTime <= elapsedTime) {
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

        v->SetVelocity(initDirection.Normalized() * 100);

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
            endDirection.x = (tasks[index].position_leave.x - tasks[index].position.x);
            endDirection.y = (tasks[index].position_leave.y - tasks[index].position.y);
            //上面是设置离开时方向。
            v->SetVelocity(endDirection.Normalized() * 100);//向离开的位置奔去
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