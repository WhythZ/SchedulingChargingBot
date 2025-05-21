#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"

//表示一辆可被充电的静止车辆（由Robot移动前来充电）
class Vehicle :public Chargeable
{
public:
    SDL_Rect chargedRect = {};                     //可被充电的有效区域（3x3瓦片）
    Chargeable* charger = nullptr;                 //当前为它充电的机器人
    Chargeable* isTargeted = nullptr;              //被哪个机器人锁定
    double TargetedDistance = 0.0;                 //被锁定时的距离
	double TargetedValue = 0.0;                    //被锁定时的最大价值

    bool isOnline = false;                         //是否正式上线，并可调度
    double arriveTime = 0;                         //车辆到达园区时间（秒）
    int vehicleNo;                                 //车辆编号

private:
    double targetElectricity = 100.0;              //离开前期望电量
    double leaveTime = 999999.0;                   //离开时间（绝对时间戳，秒）



public:
    Vehicle();
    ~Vehicle() = default;

    void OnUpdate(double) override;

    void ChangeState(std::string) override;

    bool IsBusy() const override;                  //是否正在被充电（用于调度判断）
    bool NeedElectricity() const override;         //是否需要被充电（用于调度判断）

    //电量接口
    void SetElectricity(double);
    double GetElectricity() const;

    //目标电量接口
    void SetTargetElectricity(double);
    double GetTargetElectricity() const;

    //离开时间接口
    void SetLeaveTime(double);
    double GetLeaveTime() const;
};

#endif
