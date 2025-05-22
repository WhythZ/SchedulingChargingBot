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
        size_t vehicleTaskNo;                  //每辆车唯一标识符
        
        double spawnTime;                      //多久后到达园区（秒）
        double leaveTime;                      //多久后离开园区（秒）

        double spawnElectricity;               //初始具备电量（0~40）
        double leaveElectricity;               //离开所需电量（80~100）

        Vector2 targetPosition;                //目标生成位置（像素坐标）
        Vector2 spawnPosition;                 //进入园区位置（像素坐标）
        Vector2 leavePosition;                 //离开园区位置（像素坐标）
    };

private:
    ScaleLevel currentScaleLevel = ScaleLevel::Small;

    #pragma region SimulationConfig
    size_t vehicleNumLevelSmall = 100;
    size_t vehicleNumLevelMedium = 400;
    size_t vehicleNumLevelLarge = 1600;

    size_t robotNumLevelSmall = 8;
    size_t robotNumLevelMedium = 32;
    size_t robotNumLevelLarge = 128;

    size_t vehicleSpawnTimeUpperLevelSmall = 4;   //小规模下，最长间隔多久生成
    size_t vehicleSpawnTimeUpperLevelMedium = 3;  //种规模下，最长间隔多久生成
    size_t vehicleSpawnTimeUpperLevelLarge = 2;   //大规模下，最长间隔多久生成

    size_t vehicleLeaveTimeSpanLevelSmall = 15;   //小规模下，待多久未被充满就先离开
    size_t vehicleLeaveTimeSpanLevelMedium = 12;  //种规模下，待多久未被充满就先离开
    size_t vehicleLeaveTimeSpanLevelLarge = 8;    //大规模下，待多久未被充满就先离开

    size_t vehicleSpawnElectricityUpper = 40;     //初始电量上限
    size_t vehicleLeaveElectricityLower = 80;     //所需电量下限
    #pragma endregion

    #pragma region VehicleQueue
    double elapsedTime = 0;                       //累计运行时间
    size_t nextIndex = 0;                         //下一个生成任务索引

    size_t hitVehicleNum = 0;                     //完成充电的车数量
    size_t missVehicleNum = 0;                    //未完成而离开的车数量

    std::vector<VehicleSpawnTask> tasks;          //所有车辆生成配置任务
    std::queue<Vehicle*> pendingQueue;            //尚未上线的车辆队列
    std::queue<Vehicle*> comingQueue;             //正在前往目的地的车辆队列
    std::queue<Vehicle*> workingQueue;            //等待或充电中的车辆队列
    std::queue<Vehicle*> leavingQueue;            //正在离开的车辆队列
    #pragma endregion

public:
    void OnUpdate(double);

    void LoadSimulationConfig();                  //加载配置信息
    void ChangeLevel(ScaleLevel);                 //加载不同规模

    ScaleLevel GetCurrentLevel() const;           //获取当前规模
    size_t GetHitVehicleNum() const;
    size_t GetMissVehicleNum() const;

private:
	SpawnManager() = default;
	~SpawnManager() = default;

    void RefreshVehicleTasks();                   //清空所有任务
    void UpdateVehicleSpawn(double);              //处理载具的到达与上线
    void ChangeVehicleLevel(ScaleLevel);
    void ChangeRobotLevel(ScaleLevel);
};

#endif