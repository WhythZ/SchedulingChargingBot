#include "../../../Header/Manager/Concrete/VehicleSpawner.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Tilemap/Tile.h"

#include <cstdlib>
#include <set>
#include <cstdio>

// ���벻ͬ�����ȼ���Ӧ�ĳ�����������
void VehicleSpawner::LoadScenario(int level)
{
    tasks.clear();       // ��վ�����
    nextIndex = 0;       // ��������
    elapsedTime = 0;     // ���÷���ʱ��

    while (!pendingQueue.empty()) pendingQueue.pop(); // ��յȴ�����

    // �����Ѷȵȼ�����Ҫ���ɵĳ�������
    int vehicleCount = (level == 0) ? 5 : (level == 1) ? 10 : 20;

    // ��̬��ȡ��ͼ����Ƭ��������������
    int mapTilesX = SceneManager::Instance()->mapRect.w / TILE_SIZE;
    int mapTilesY = SceneManager::Instance()->mapRect.h / TILE_SIZE;

    std::set<std::pair<int, int>> occupiedTiles;  // ��¼������Ƭ�������ظ�����

    for (int i = 0; i < vehicleCount; ++i) {
        VehicleSpawnTask task;

        // ���ó�������ʱ�䣺���Ƴٲ�������Ŷ������⼯������
        task.spawnTime = rand() % 10 + i * 3;

        int tileX, tileY;
        // ���ѡ��δռ�õ���Ƭ���ܿ���ͼ��Ե2��
        do {
            tileX = rand() % (mapTilesX - 4) + 2;
            tileY = rand() % (mapTilesY - 4) + 2;
        } while (occupiedTiles.count({ tileX, tileY }));  // ȷ�����ظ�

        occupiedTiles.insert({ tileX, tileY });  // ��¼�����Ƭ�ѱ�ʹ��

        // ���ó�����ʵ��λ�ã�ת��Ϊ�������꣬��������Ƭ��
        task.position = {
            static_cast<double>(tileX * TILE_SIZE + TILE_SIZE / 2),
            static_cast<double>(tileY * TILE_SIZE + TILE_SIZE / 2)
        };

        // ��ʼ����Ϊ 20%~59%
        task.initialElectricity = rand() % 40 + 20;

        // �뿪ʱҪ��ĵ���Ϊ 80%~100%
        task.requiredElectricity = 80 + rand() % 21;

        // �뿪ʱ��Ϊ����ʱ��� 60~120 ��
        task.leaveTime = task.spawnTime + 60 + rand() % 61;

        tasks.push_back(task);  // ���������б�
    }
}

// ��ÿһ֡����ʱ���ã�����������������
void VehicleSpawner::OnUpdate(double delta)
{
    elapsedTime += delta;  // �ۼӷ���ʱ��

    // �ѡ�����ʱ���ѵ����������ɴ����߳���
    while (nextIndex < tasks.size() && tasks[nextIndex].spawnTime <= elapsedTime) {
        const auto& t = tasks[nextIndex];

        Vehicle* v = new Vehicle();
        v->SetPosition((int)t.position.x, (int)t.position.y);
        v->SetElectricity(t.initialElectricity);
        v->SetTargetElectricity(t.requiredElectricity);
        v->SetLeaveTime(t.leaveTime);

        v->isOnline = false;     // �յ����δ��ʽ�������
        v->arriveTime = t.spawnTime;

        pendingQueue.push(v);    // ����ȴ����߶���
        ++nextIndex;
    }

    // ��� pendingQueue ����Щ��������ʽ����
    size_t queueSize = pendingQueue.size();
    for (size_t i = 0; i < queueSize; ++i) {
        Vehicle* v = pendingQueue.front();
        pendingQueue.pop();

        if (v->arriveTime <= elapsedTime) {
            // ����ʱ������ �� �������ϵͳ
            v->isOnline = true;
            ChargeableManager::Instance()->AddChargeable(v);

            printf("[ARRIVE] Vehicle at (%.0f, %.0f), elec=%zu%%, needs=%zu%%, leave=%.0fs\n",
                v->GetPosition().x, v->GetPosition().y,
                v->GetElectricity(), v->GetTargetElectricity(), v->GetLeaveTime());
        }
        else {
            // ��û���� �� �������ڵȴ�������
            pendingQueue.push(v);
        }
    }
}
