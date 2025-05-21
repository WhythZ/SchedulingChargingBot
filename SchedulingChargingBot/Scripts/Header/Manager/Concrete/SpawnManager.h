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
        size_t vehicleTaskNo;                //每辆车唯一标识符
        
        double spawnTime;                    //多久后到达园区（秒）
        double leaveTime;                    //多久后离开园区（秒）

        double spawnElectricity;             //初始具备电量（0~40）
        double leaveElectricity;             //离开所需电量（80~100）

        Vector2 targetPosition;              //目标生成位置（像素坐标）
        Vector2 spawnPosition;               //进入园区位置（像素坐标）
        Vector2 leavePosition;               //离开园区位置（像素坐标）
    };

private:
    ScaleLevel currentScaleLevel = ScaleLevel::Small;

    #pragma region VehicleTask
    double elapsedTime = 0;                  //累计运行时间

    size_t spawnTimeUpper = 2;               //最长间隔多久生成
    size_t leaveTimeSpan = 30;               //待多久未被充满就先离开

    size_t spawnElectricityUpper = 40;       //初始电量上限
    size_t leaveElectricityLower = 80;       //所需电量下限
    #pragma endregion

    #pragma region VehicleQueue
    size_t nextIndex = 0;                    //下一个生成任务索引

    std::vector<VehicleSpawnTask> tasks;     //所有车辆生成配置任务
    std::queue<Vehicle*> pendingQueue;       //尚未上线的车辆队列
    std::queue<Vehicle*> comingQueue;        //正在前往目的地的车辆队列
    std::queue<Vehicle*> workingQueue;       //等待或充电中的车辆队列
    std::queue<Vehicle*> leavingQueue;       //正在离开的车辆队列
    #pragma endregion

    #pragma region ScaleLevelNum
    size_t vehicleNumLevelSmall = 100;
    size_t vehicleNumLevelMedium = 400;
    size_t vehicleNumLevelLarge = 1600;

    size_t robotNumLevelSmall = 4;
    size_t robotNumLevelMedium = 16;
    size_t robotNumLevelLarge = 64;
    #pragma endregion

public:
    void OnUpdate(double);

    void ChangeLevel(ScaleLevel);            //加载不同规模

    ScaleLevel GetCurrentLevel() const;      //获取当前规模

private:
	SpawnManager() = default;
	~SpawnManager() = default;

    void RefreshVehicleTasks();              //清空所有任务
    void UpdateVehicleSpawn(double);         //处理载具的到达与上线
    void ChangeVehicleLevel(ScaleLevel);
    void ChangeRobotLevel(ScaleLevel);
};

#endif