#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"
#include <atomic>

// ��ʾһ���ɱ����ľ�ֹ�������� Robot �ƶ�ǰ����磩
class Vehicle : public Chargeable
{
public:
    SDL_Rect chargedRect = {};                     // �ɱ�������Ч����3x3��Ƭ��
    Chargeable* charger = nullptr;                 // ��ǰΪ�����Ļ�����
    std::atomic<Chargeable*> isTargeted = nullptr; // ���ĸ�����������
    double TargetedDistance = 0.0;                 // ������ʱ�ľ���

    bool isOnline = false;                         // �Ƿ���ʽ�����ߡ����ɵ���
    double arriveTime = 0;                         // ��������԰��ʱ�䣨�룩

public:
    Vehicle();
    ~Vehicle() override = default;

    //ÿ֡����
    void OnUpdate(double delta) override;

    //���Ķ���״̬
    void ChangeState(std::string stateName) override;

    //�Ƿ�������磨���ڵ����жϣ�
    bool IsBusy() const override;

    //�����ӿ�
    void SetElectricity(double);
    double GetElectricity() const;

    //Ŀ������ӿ�
    void SetTargetElectricity(double);
    double GetTargetElectricity() const;

    //�뿪ʱ��ӿ�
    void SetLeaveTime(double);
    double GetLeaveTime() const;

private:
    double targetElectricity = 100;              //�뿪ǰ��������
    double leaveTime = 999999;                   //�뿪ʱ�䣨����ʱ������룩
};

#endif
