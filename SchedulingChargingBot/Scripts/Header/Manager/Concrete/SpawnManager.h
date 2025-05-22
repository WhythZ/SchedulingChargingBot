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
        size_t vehicleTaskNo;                  //ÿ����Ψһ��ʶ��
        
        double spawnTime;                      //��ú󵽴�԰�����룩
        double leaveTime;                      //��ú��뿪԰�����룩

        double spawnElectricity;               //��ʼ�߱�������0~40��
        double leaveElectricity;               //�뿪���������80~100��

        Vector2 targetPosition;                //Ŀ������λ�ã��������꣩
        Vector2 spawnPosition;                 //����԰��λ�ã��������꣩
        Vector2 leavePosition;                 //�뿪԰��λ�ã��������꣩
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

    size_t vehicleSpawnTimeUpperLevelSmall = 4;   //С��ģ�£������������
    size_t vehicleSpawnTimeUpperLevelMedium = 3;  //�ֹ�ģ�£������������
    size_t vehicleSpawnTimeUpperLevelLarge = 2;   //���ģ�£������������

    size_t vehicleLeaveTimeSpanLevelSmall = 15;   //С��ģ�£������δ�����������뿪
    size_t vehicleLeaveTimeSpanLevelMedium = 12;  //�ֹ�ģ�£������δ�����������뿪
    size_t vehicleLeaveTimeSpanLevelLarge = 8;    //���ģ�£������δ�����������뿪

    size_t vehicleSpawnElectricityUpper = 40;     //��ʼ��������
    size_t vehicleLeaveElectricityLower = 80;     //�����������
    #pragma endregion

    #pragma region VehicleQueue
    double elapsedTime = 0;                       //�ۼ�����ʱ��
    size_t nextIndex = 0;                         //��һ��������������

    size_t hitVehicleNum = 0;                     //��ɳ��ĳ�����
    size_t missVehicleNum = 0;                    //δ��ɶ��뿪�ĳ�����

    std::vector<VehicleSpawnTask> tasks;          //���г���������������
    std::queue<Vehicle*> pendingQueue;            //��δ���ߵĳ�������
    std::queue<Vehicle*> comingQueue;             //����ǰ��Ŀ�ĵصĳ�������
    std::queue<Vehicle*> workingQueue;            //�ȴ������еĳ�������
    std::queue<Vehicle*> leavingQueue;            //�����뿪�ĳ�������
    #pragma endregion

public:
    void OnUpdate(double);

    void LoadSimulationConfig();                  //����������Ϣ
    void ChangeLevel(ScaleLevel);                 //���ز�ͬ��ģ

    ScaleLevel GetCurrentLevel() const;           //��ȡ��ǰ��ģ
    size_t GetHitVehicleNum() const;
    size_t GetMissVehicleNum() const;

private:
	SpawnManager() = default;
	~SpawnManager() = default;

    void RefreshVehicleTasks();                   //�����������
    void UpdateVehicleSpawn(double);              //�����ؾߵĵ���������
    void ChangeVehicleLevel(ScaleLevel);
    void ChangeRobotLevel(ScaleLevel);
};

#endif