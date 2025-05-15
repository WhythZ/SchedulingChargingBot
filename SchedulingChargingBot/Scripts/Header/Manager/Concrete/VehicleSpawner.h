#ifndef _VEHICLE_SPAWNER_H_
#define _VEHICLE_SPAWNER_H_

#include <vector>
#include <queue>
#include "../../Chargeable/Concrete/Vehicle.h"

// ��ʾÿ��������ʱ����������
struct VehicleSpawnTask {
    double spawnTime;             // ����԰��ʱ�䣨�룩
    Vector2 position;             // ��ʼ����λ�ã��������꣩
    double initialElectricity;    // ��ʼ������0-100��
    double requiredElectricity;   // �뿪ǰ��Ҫ����
    double leaveTime;             // �뿪԰��ʱ�䣨�룩
};

class VehicleSpawner {
private:
    std::vector<VehicleSpawnTask> tasks;  // ���г���������������
    std::queue<Vehicle*> pendingQueue;    // ��δ���ߵĳ�������
    size_t nextIndex = 0;                 // ��һ�� spawn ��������
    double elapsedTime = 0;               // �ۼ�����ʱ��

public:
    void LoadScenario(int level);         // ���ز�ͬ��ģ
    void OnUpdate(double delta);          // ÿ֡��������/�����߼�
};

#endif
