#ifndef _SPAWN_MANAGER_H_
#define _SPAWN_MANAGER_H_

#include <vector>
#include <queue>
#include "../Manager.hpp"
#include "../../Chargeable/Concrete/Vehicle.h"

class SpawnManager :public Manager<SpawnManager>
{
	friend Manager<SpawnManager>;

public:
    enum class ScaleLevel
    {
        Small,
        Medium,
        Large
    };

private:
    //表示每辆车生成时的配置数据
    struct VehicleSpawnTask
    {
        double spawnTime;                    //到达园区时间（秒）
        Vector2 targetPosition;              //目标生成位置（像素坐标）
        Vector2 spawnPosition;               //进入园区位置（像素坐标）
        Vector2 leavePosition;               //离开园区位置（像素坐标）
        double initialElectricity;           //初始电量（0-100）
        double requiredElectricity;          //离开前需要电量
        double leaveTime;                    //离开园区时间（秒）
        int VehicleTaskNo;                   //每辆车唯一标识符
    };

private:
    ScaleLevel currentScaleLevel = ScaleLevel::Small;

    #pragma region VehicleSpawnData
    std::vector<VehicleSpawnTask> tasks;     //所有车辆生成配置任务
    std::queue<Vehicle*> pendingQueue;       //尚未上线的车辆队列
    std::queue<Vehicle*> comingQueue;        //正在前往目的地的车辆队列
    std::queue<Vehicle*> workingQueue;       //等待或充电中的车辆队列
    std::queue<Vehicle*> leavingQueue;       //正在离开的车辆队列

    size_t nextIndex = 0;                    //下一个生成任务索引
    double elapsedTime = 0;                  //累计运行时间
    #pragma endregion

public:
    void OnUpdate(double);

    void ChangeLevel(ScaleLevel);            //加载不同规模

    ScaleLevel GetCurrentLevel() const;      //获取当前规模

private:
	SpawnManager() = default;
	~SpawnManager() = default;

    void UpdateVehicleSpawn(double);         //处理载具的到达与上线
    void LoadVehicleLevel(ScaleLevel);
    void LoadRobotLevel(ScaleLevel);
    void RefreshVehicleTasks();              //清空所有任务
};

#endif