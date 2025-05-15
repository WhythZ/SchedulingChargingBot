#ifndef _VEHICLE_SPAWNER_H_
#define _VEHICLE_SPAWNER_H_

#include <vector>
#include <queue>
#include "../../Chargeable/Concrete/Vehicle.h"

// 表示每辆车生成时的配置数据
struct VehicleSpawnTask {
    double spawnTime;             // 到达园区时间（秒）
    Vector2 position;             // 初始生成位置（像素坐标）
    double initialElectricity;    // 初始电量（0-100）
    double requiredElectricity;   // 离开前需要电量
    double leaveTime;             // 离开园区时间（秒）
};

class VehicleSpawner {
private:
    std::vector<VehicleSpawnTask> tasks;  // 所有车辆生成配置任务
    std::queue<Vehicle*> pendingQueue;    // 尚未上线的车辆队列
    size_t nextIndex = 0;                 // 下一个 spawn 任务索引
    double elapsedTime = 0;               // 累计运行时间

public:
    void LoadScenario(int level);         // 加载不同规模
    void OnUpdate(double delta);          // 每帧更新生成/上线逻辑
};

#endif
