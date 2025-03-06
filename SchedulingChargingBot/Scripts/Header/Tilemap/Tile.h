#ifndef _TILE_H_
#define _TILE_H_

#include <vector>

//宏定义每个矩形瓦片的边长（单位为像素）
#define TILE_SIZE 64

struct Tile
{
	#pragma region LoadFromCSV
	int tilesetIdx = 0;                      //确定该瓦片从瓦片纹理集中选取哪一个图片
	int functionFlag = 0;                    //0表示瓦片无功能，正数1开始表示车辆生成点，负数-1开始表示充电桩
	#pragma endregion
};

//自定义二维瓦片集类型
typedef std::vector<std::vector<Tile>> TileMap;

#endif