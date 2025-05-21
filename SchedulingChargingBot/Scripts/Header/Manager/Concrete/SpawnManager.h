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
        size_t vehicleTaskNo;                //ÿ����Ψһ��ʶ��
        
        double spawnTime;                    //��ú󵽴�԰�����룩
        double leaveTime;                    //��ú��뿪԰�����룩

        double spawnElectricity;             //��ʼ�߱�������0~40��
        double leaveElectricity;             //�뿪���������80~100��

        Vector2 targetPosition;              //Ŀ������λ�ã��������꣩
        Vector2 spawnPosition;               //����԰��λ�ã��������꣩
        Vector2 leavePosition;               //�뿪԰��λ�ã��������꣩
    };

private:
    ScaleLevel currentScaleLevel = ScaleLevel::Small;

    #pragma region VehicleTask
    double elapsedTime = 0;                  //�ۼ�����ʱ��

    size_t spawnTimeUpper = 2;               //�����������
    size_t leaveTimeSpan = 30;               //�����δ�����������뿪

    size_t spawnElectricityUpper = 40;       //��ʼ��������
    size_t leaveElectricityLower = 80;       //�����������
    #pragma endregion

    #pragma region VehicleQueue
    size_t nextIndex = 0;                    //��һ��������������

    std::vector<VehicleSpawnTask> tasks;     //���г���������������
    std::queue<Vehicle*> pendingQueue;       //��δ���ߵĳ�������
    std::queue<Vehicle*> comingQueue;        //����ǰ��Ŀ�ĵصĳ�������
    std::queue<Vehicle*> workingQueue;       //�ȴ������еĳ�������
    std::queue<Vehicle*> leavingQueue;       //�����뿪�ĳ�������
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

    void ChangeLevel(ScaleLevel);            //���ز�ͬ��ģ

    ScaleLevel GetCurrentLevel() const;      //��ȡ��ǰ��ģ

private:
	SpawnManager() = default;
	~SpawnManager() = default;

    void RefreshVehicleTasks();              //�����������
    void UpdateVehicleSpawn(double);         //�����ؾߵĵ���������
    void ChangeVehicleLevel(ScaleLevel);
    void ChangeRobotLevel(ScaleLevel);
};

#endif