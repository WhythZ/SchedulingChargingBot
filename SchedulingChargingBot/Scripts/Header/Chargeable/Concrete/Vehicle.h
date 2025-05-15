#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"
#include <atomic>

// 表示一辆可被充电的静止车辆（由 Robot 移动前来充电）
class Vehicle : public Chargeable
{
public:
    SDL_Rect chargedRect = {};                     // 可被充电的有效区域（3x3瓦片）
    Chargeable* charger = nullptr;                 // 当前为它充电的机器人
    std::atomic<Chargeable*> isTargeted = nullptr; // 被哪个机器人锁定
    double TargetedDistance = 0.0;                 // 被锁定时的距离

    bool isOnline = false;                         // 是否正式“上线”并可调度
    double arriveTime = 0;                         // 车辆到达园区时间（秒）

public:
    Vehicle();
    ~Vehicle() override = default;

    //每帧更新
    void OnUpdate(double delta) override;

    //更改动画状态
    void ChangeState(std::string stateName) override;

    //是否正被充电（用于调度判断）
    bool IsBusy() const override;

    //电量接口
    void SetElectricity(double);
    double GetElectricity() const;

    //目标电量接口
    void SetTargetElectricity(double);
    double GetTargetElectricity() const;

    //离开时间接口
    void SetLeaveTime(double);
    double GetLeaveTime() const;

private:
    double targetElectricity = 100;              //离开前期望电量
    double leaveTime = 999999;                   //离开时间（绝对时间戳，秒）
};

#endif
