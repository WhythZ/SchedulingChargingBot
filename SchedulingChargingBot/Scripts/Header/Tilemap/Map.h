#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include <unordered_map>
#include <string>
#include "Tile.h"

class Map
{
private:
	TileMap tileMap;                                       //记录瓦片地图
	std::unordered_map<size_t, SDL_Point> stationIdxPool;  //记录充电桩坐标
	std::unordered_map<size_t, SDL_Point> vehicleIdxPool;  //记录车辆生成点坐标

public:
	Map() = default;
	~Map() = default;

	bool Load(const std::string&);

	TileMap GetTileMap() const;
	std::unordered_map<size_t, SDL_Point> GetStationIdxPool() const;
	std::unordered_map<size_t, SDL_Point> GetVehicleIdxPool() const;
	size_t GetWidthTileNum() const;
	size_t GetHeightTileNum() const;

private:
	std::string TrimString(const std::string);             //剪切瓦片字符串以保证格式的统一以便读取
	void LoadTileFromString(const std::string, Tile&);     //从瓦片字符串中提取信息并载入
	void GenerateMapCache();                               //用于读取生成地图的缓存
};

#endif