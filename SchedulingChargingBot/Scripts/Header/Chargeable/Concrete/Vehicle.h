#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Chargeable.h"

//��ʾһ���ɱ����ľ�ֹ��������Robot�ƶ�ǰ����磩
class Vehicle :public Chargeable
{
public:
    SDL_Rect chargedRect = {};                     //�ɱ�������Ч����3x3��Ƭ��
    Chargeable* charger = nullptr;                 //��ǰΪ�����Ļ�����
    Chargeable* isTargeted = nullptr;              //���ĸ�����������
    double TargetedDistance = 0.0;                 //������ʱ�ľ���
	double TargetedValue = 0.0;                    //������ʱ������ֵ

    bool isOnline = false;                         //�Ƿ���ʽ���ߣ����ɵ���
    double arriveTime = 0;                         //��������԰��ʱ�䣨�룩
    int vehicleNo;                                 //�������

private:
    double targetElectricity = 100.0;              //�뿪ǰ��������
    double leaveTime = 999999.0;                   //�뿪ʱ�䣨����ʱ������룩



public:
    Vehicle();
    ~Vehicle() = default;

    void OnUpdate(double) override;

    void ChangeState(std::string) override;

    bool IsBusy() const override;                  //�Ƿ����ڱ���磨���ڵ����жϣ�
    bool NeedElectricity() const override;         //�Ƿ���Ҫ����磨���ڵ����жϣ�

    //�����ӿ�
    void SetElectricity(double);
    double GetElectricity() const;

    //Ŀ������ӿ�
    void SetTargetElectricity(double);
    double GetTargetElectricity() const;

    //�뿪ʱ��ӿ�
    void SetLeaveTime(double);
    double GetLeaveTime() const;
};

#endif
