#ifndef _VEHICLE_SPAWNER_H_
#define _VEHICLE_SPAWNER_H_

#include <vector>
#include <queue>
#include "../../Chargeable/Concrete/Vehicle.h"

// ��ʾÿ��������ʱ����������
struct VehicleSpawnTask {
    double spawnTime;             // ����԰��ʱ�䣨�룩
    Vector2 position;             // Ŀ������λ�ã��������꣩
    Vector2 position_spawn;       // ����԰��λ�ã��������꣩
    Vector2 position_leave;       // �뿪԰��λ�ã��������꣩
    double initialElectricity;    // ��ʼ������0-100��
    double requiredElectricity;   // �뿪ǰ��Ҫ����
    double leaveTime;             // �뿪԰��ʱ�䣨�룩
    int VehicleTaskNo;            // ÿ����Ψһ��ʶ��
};

class VehicleSpawner {
private:
    std::vector<VehicleSpawnTask> tasks;  // ���г���������������
    std::queue<Vehicle*> pendingQueue;    // ��δ���ߵĳ�������
    std::queue<Vehicle*> comingQueue;     // ����ǰ��Ŀ�ĵصĳ�������
    std::queue<Vehicle*> workingQueue;    // �ȴ������еĳ�������
    std::queue<Vehicle*> leavingQueue;    // �����뿪�ĳ�������

    size_t nextIndex = 0;                 // ��һ�� spawn ��������
    double elapsedTime = 0;               // �ۼ�����ʱ��
public:
    void LoadScenario(int level);         // ���ز�ͬ��ģ
    void OnUpdate(double delta);          // ÿ֡��������/�����߼�
};

#endif

