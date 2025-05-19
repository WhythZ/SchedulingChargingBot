#ifndef _VEHICLE_SPAWNER_H_
#define _VEHICLE_SPAWNER_H_

#include <vector>
#include <queue>
#include "../../Chargeable/Concrete/Vehicle.h"
#include "../../Manager/Manager.hpp"

// 表示每辆车生成时的配置数据
struct VehicleSpawnTask {
    double spawnTime;             // 到达园区时间（秒）
    Vector2 position;             // 目标生成位置（像素坐标）
    Vector2 position_spawn;       // 进入园区位置（像素坐标）
    Vector2 position_leave;       // 离开园区位置（像素坐标）
    double initialElectricity;    // 初始电量（0-100）
    double requiredElectricity;   // 离开前需要电量
    double leaveTime;             // 离开园区时间（秒）
    int VehicleTaskNo;            // 每辆车唯一标识符
};

class VehicleSpawner : public Manager<VehicleSpawner> {
    friend class Manager<VehicleSpawner>;
private:
    std::vector<VehicleSpawnTask> tasks;  // 所有车辆生成配置任务
    std::queue<Vehicle*> pendingQueue;    // 尚未上线的车辆队列
    std::queue<Vehicle*> comingQueue;     // 正在前往目的地的车辆队列
    std::queue<Vehicle*> workingQueue;    // 等待或充电中的车辆队列
    std::queue<Vehicle*> leavingQueue;    // 正在离开的车辆队列

    size_t nextIndex = 0;                 // 下一个 spawn 任务索引
    double elapsedTime = 0;               // 累计运行时间
    int totalSpawned = 0;                 //总共生成的车辆数
    int totalLeft = 0;                    //离开的车辆数目
public:
    void LoadScenario(int level);         // 加载不同规模
    void OnUpdate(double delta);          // 每帧更新生成/上线逻辑
    void ClearAllVehicles();              //方便在切换模式的时候清空

    int GetTotalSpawned() const { return totalSpawned; }
    int GetTotalLeft() const { return totalLeft; }
};

#endif

