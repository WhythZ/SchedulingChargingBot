#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include <map>
#include <string>
#include "Tile.h"

class Map
{
private:
	TileMap tileMap;                                       //记录瓦片地图

	std::map<size_t, SDL_Point> stationIdxPool;            //记录充电桩坐标
	std::map<size_t, SDL_Point> vehicleIdxPool;            //记录车辆生成点坐标

	std::vector<SDL_Rect> stationRects;                    //记录各充电桩作用范围
	std::vector<SDL_Rect> vehicleRects;                    //记录各车辆生成点作用范围

public:
	Map() = default;
	~Map() = default;

	bool Load(const std::string&);

	TileMap GetTileMap() const;
	const std::map<size_t, SDL_Point>& GetStationIdxPool() const;
	const std::map<size_t, SDL_Point>& GetVehicleIdxPool() const;
	size_t GetWidthTileNum() const;
	size_t GetHeightTileNum() const;
	const std::vector<SDL_Rect>& GetStationRects() const;
	const std::vector<SDL_Rect>& GetVehicleRects() const;

private:
	std::string TrimString(const std::string);             //剪切瓦片字符串以保证格式的统一以便读取
	void LoadTileFromString(const std::string, Tile&);     //从瓦片字符串中提取信息并载入
	void GenerateMapCache();                               //用于读取生成地图的缓存
};

#endif