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
    //��ʾÿ��������ʱ����������
    struct VehicleSpawnTask
    {
        double spawnTime;                    //����԰��ʱ�䣨�룩
        Vector2 position;                    //Ŀ������λ�ã��������꣩
        Vector2 position_spawn;              //����԰��λ�ã��������꣩
        Vector2 position_leave;              //�뿪԰��λ�ã��������꣩
        double initialElectricity;           //��ʼ������0-100��
        double requiredElectricity;          //�뿪ǰ��Ҫ����
        double leaveTime;                    //�뿪԰��ʱ�䣨�룩
        int VehicleTaskNo;                   //ÿ����Ψһ��ʶ��
    };

private:
    ScaleLevel currentScaleLevel;

    #pragma region VehicleSpawnData
    std::vector<VehicleSpawnTask> tasks;     //���г���������������
    std::queue<Vehicle*> pendingQueue;       //��δ���ߵĳ�������
    std::queue<Vehicle*> comingQueue;        //����ǰ��Ŀ�ĵصĳ�������
    std::queue<Vehicle*> workingQueue;       //�ȴ������еĳ�������
    std::queue<Vehicle*> leavingQueue;       //�����뿪�ĳ�������

    size_t nextIndex = 0;                    //��һ��spawn��������
    double elapsedTime = 0;                  //�ۼ�����ʱ��
    int totalSpawned = 0;                    //�ܹ����ɵĳ�����
    int totalLeft = 0;                       //�뿪�ĳ�����Ŀ
    #pragma endregion

public:
    void OnUpdate(double);

    void ChangeLevel(ScaleLevel);            //���ز�ͬ��ģ

    ScaleLevel GetCurrentScaleLevel() const; //��ȡ��ǰ��ģ
    int GetTotalSpawned() const;
    int GetTotalLeft() const;

private:
	SpawnManager() = default;
	~SpawnManager() = default;

    void UpdateVehicleSpawn(double);         //�����ؾߵĵ���������
    void UpdateRobotSpawn(double);

    void LoadVehicleLevel(ScaleLevel);
    void LoadRobotLevel(ScaleLevel);
};

#endif