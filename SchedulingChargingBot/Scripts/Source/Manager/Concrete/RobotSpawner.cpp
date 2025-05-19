#include "../../../Header/Manager/Concrete/RobotSpawner.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Tilemap/Tile.h"

#include <cstdlib>
#include <set>
#include <cstdio>
#include<random>

// 载入不同场景等级对应的车辆生成任务
void RobotSpawner::CreateRobot(int level)
{

    // 由level控制充电机器人的数量
    int Robotnum = (level == 0) ? 3 : (level == 1) ? 6 : 9; 

    double posx, posy;//记录出生位置的x和y坐标信息
    

    std::set<std::pair<int, int>> occupiedTiles;  // 记录已用瓦片，避免重复生成

    for (int i = 0; i < Robotnum; i++)
    {
        int tx,ty;
        do
        {
            BornPlaceCreate(posx,posy,tx,ty);//posx和posy接受随机生成的位置信息
        } while (occupiedTiles.count({ tx, ty }));

        occupiedTiles.insert({ tx, ty });  // 记录这个瓦片已被使用

		ChargeableManager::Instance()->SpawnChargeableAt(ChargeableType::Robot, { tx, ty });
        Robot* r = new Robot();        

        printf("[CREAT]A ROBOT HAS CREAT AT (%.0f, %.0f)\n",posx,posy);
    }
}

void RobotSpawner::BornPlaceCreate(double& x, double& y, int& tilex, int& tiley)
{
    // 创建随机数引擎（使用硬件随机数种子）
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister 引擎，种子为 rd()

    // 创建均匀整数分布（包含0~3）
    std::uniform_int_distribution<> dist(0, 3);

    // 动态获取地图的瓦片数量（行列数）
    int mapTilesX = SceneManager::Instance()->mapRect.w / TILE_SIZE;
    int mapTilesY = SceneManager::Instance()->mapRect.h / TILE_SIZE;

    int BornCharger1 = dist(gen);//决定哪一个角落
    
    int BornCharger2 = dist(gen);//决定充电桩中的位置
    int Charger_x = BornCharger2 % 2;
    int Charger_y = (BornCharger2 ) / 2;
    //int Charger_y = 0;
    //int tilepos;
    printf("(%d,%d)", BornCharger1,BornCharger2);
    //double position_x=0.0, position_y=0.0;//出生位置
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
