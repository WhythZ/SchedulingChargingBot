#include "../../../Header/Manager/Concrete/RobotSpawner.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Tilemap/Tile.h"

#include <cstdlib>
#include <set>
#include <cstdio>
#include<random>

// ���벻ͬ�����ȼ���Ӧ�ĳ�����������
void RobotSpawner::CreateRobot(int level)
{

    // ��level���Ƴ������˵�����
    int Robotnum = (level == 0) ? 3 : (level == 1) ? 6 : 9; 

    double posx, posy;//��¼����λ�õ�x��y������Ϣ
    

    std::set<std::pair<int, int>> occupiedTiles;  // ��¼������Ƭ�������ظ�����

    for (int i = 0; i < Robotnum; i++)
    {
        int tx,ty;
        do
        {
            BornPlaceCreate(posx,posy,tx,ty);//posx��posy����������ɵ�λ����Ϣ
        } while (occupiedTiles.count({ tx, ty }));

        occupiedTiles.insert({ tx, ty });  // ��¼�����Ƭ�ѱ�ʹ��

		ChargeableManager::Instance()->SpawnChargeableAt(ChargeableType::Robot, { tx, ty });
        Robot* r = new Robot();        

        printf("[CREAT]A ROBOT HAS CREAT AT (%.0f, %.0f)\n",posx,posy);
    }
}

void RobotSpawner::BornPlaceCreate(double& x, double& y, int& tilex, int& tiley)
{
    // ������������棨ʹ��Ӳ����������ӣ�
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister ���棬����Ϊ rd()

    // �������������ֲ�������0~3��
    std::uniform_int_distribution<> dist(0, 3);

    // ��̬��ȡ��ͼ����Ƭ��������������
    int mapTilesX = SceneManager::Instance()->mapRect.w / TILE_SIZE;
    int mapTilesY = SceneManager::Instance()->mapRect.h / TILE_SIZE;

    int BornCharger1 = dist(gen);//������һ������
    
    int BornCharger2 = dist(gen);//�������׮�е�λ��
    int Charger_x = BornCharger2 % 2;
    int Charger_y = (BornCharger2 ) / 2;
    //int Charger_y = 0;
    //int tilepos;
    printf("(%d,%d)", BornCharger1,BornCharger2);
    //double position_x=0.0, position_y=0.0;//����λ��
    double sizetile = 64.0;

    switch (BornCharger1)
    {
        case 0:
            x = sizetile * Charger_x + sizetile / 2;
            y = sizetile * Charger_y + sizetile / 2;
            tilex = Charger_x;
            tiley = Charger_y;
            break;
            
        case 1:
            x = sizetile * (mapTilesX - 2) + sizetile * Charger_x + sizetile / 2;
            y = sizetile * Charger_y + sizetile / 2;
            tilex = mapTilesX - 2 + Charger_x;
            tiley = Charger_y;
            break;
        case 2:
            x = sizetile * Charger_x + sizetile / 2;
            y = sizetile * (mapTilesY - 2) + sizetile * Charger_y + sizetile / 2;
            tilex = Charger_x;
            tiley = Charger_y + mapTilesY - 2;
            break;
        case 3:
            x = sizetile * (mapTilesX - 2) + sizetile * Charger_x + sizetile / 2;
            y = sizetile * (mapTilesY - 2) + sizetile * Charger_y + sizetile / 2;
            tilex = mapTilesX - 2 + Charger_x;
            tiley = Charger_y + mapTilesY - 2;
            break;
    }
}
