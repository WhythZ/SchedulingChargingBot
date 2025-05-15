#include "../../../Header/Manager/Concrete/VehicleSpawner.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Tilemap/Tile.h"

#include <cstdlib>
#include <set>
#include <cstdio>

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

        // 设置车辆到达时间：逐车推迟并加随机扰动，避免集中生成
        task.spawnTime = rand() % 10 + i * 3;

        int tileX, tileY;
        // 随机选择未占用的瓦片，避开地图边缘2格
        do {
            tileX = rand() % (mapTilesX - 4) + 2;
            tileY = rand() % (mapTilesY - 4) + 2;
        } while (occupiedTiles.count({ tileX, tileY }));  // 确保不重复

        occupiedTiles.insert({ tileX, tileY });  // 记录这个瓦片已被使用

        // 设置车辆的实际位置（转换为像素坐标，居中于瓦片）
        task.position = {
            static_cast<double>(tileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tileY * TILE_SIZE + TILE_SIZE / 2)
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
void VehicleSpawner::OnUpdate(double delta)
{
    elapsedTime += delta;  // 累加仿真时间

    // 把“到达时间已到”的任务变成待上线车辆
    while (nextIndex < tasks.size() && tasks[nextIndex].spawnTime <= elapsedTime) {
        const auto& t = tasks[nextIndex];

        Vehicle* v = new Vehicle();
        v->SetPosition((int)t.position.x, (int)t.position.y);
        v->SetElectricity(t.initialElectricity);
        v->SetTargetElectricity(t.requiredElectricity);
        v->SetLeaveTime(t.leaveTime);

        v->isOnline = false;     // 刚到达，还未正式进入调度
        v->arriveTime = t.spawnTime;

        pendingQueue.push(v);    // 放入等待上线队列
        ++nextIndex;
    }

    // 检查 pendingQueue 中哪些车可以正式上线
    size_t queueSize = pendingQueue.size();
    for (size_t i = 0; i < queueSize; ++i) {
        Vehicle* v = pendingQueue.front();
        pendingQueue.pop();

        if (v->arriveTime <= elapsedTime) {
            // 到达时间满足 → 加入调度系统
            v->isOnline = true;
            ChargeableManager::Instance()->AddChargeable(v);

            printf("[ARRIVE] Vehicle at (%.0f, %.0f), elec=%zu%%, needs=%zu%%, leave=%.0fs\n",
                v->GetPosition().x, v->GetPosition().y,
                v->GetElectricity(), v->GetTargetElectricity(), v->GetLeaveTime());
        }
        else {
            // 还没到达 → 继续留在等待队列中
            pendingQueue.push(v);
        }
    }
}
