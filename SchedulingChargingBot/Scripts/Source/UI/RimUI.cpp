#include "../../Header/UI/RimUI.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

#include <iostream>

RimUI::RimUI()
{
	const Map& _map = SceneManager::Instance()->map;
	
	#pragma region Stations
	std::map<size_t, SDL_Point> _stations = _map.GetStationIdxPool();
	std::cout << _stations.size() << std::endl;
	//注意充电桩的索引从1开始，0是无效值
	for (auto _elem : _stations)
	{
		SDL_Point _point = _elem.second;
		
		//计算对应位置瓦片中心点位置
		int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
		int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
		//以该瓦片为中心，周围3x3的矩形范围是可触发范围（充电桩给机器人充电、车辆可以触发被充电）
		SDL_Rect _rect =
		{
			_x - (TILE_SIZE / 2 + TILE_SIZE),
			_y - (TILE_SIZE / 2 + TILE_SIZE),
			TILE_SIZE * 3,
			TILE_SIZE * 3
		};
		stationRects.emplace_back(_rect);
	}
	#pragma endregion

	#pragma region Vehicles
	std::map<size_t, SDL_Point> _vehicles = _map.GetVehicleIdxPool();
	std::cout << _vehicles.size() << std::endl;
	for (auto _elem : _vehicles)
	{
		SDL_Point _point = _elem.second;

		//计算对应位置瓦片中心点位置
		int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
		int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
		//以该瓦片为中心，周围3x3的矩形范围是可触发范围（充电桩给机器人充电、车辆可以触发被充电）
		SDL_Rect _rect =
		{
			_x - (TILE_SIZE / 2 + TILE_SIZE),
			_y - (TILE_SIZE / 2 + TILE_SIZE),
			TILE_SIZE * 3,
			TILE_SIZE * 3
		};
		vehicleRects.emplace_back(_rect);
	}
	#pragma endregion
}

//void RimUI::OnUpdate(SDL_Renderer* _renderer)
//{
//}

void RimUI::OnRender(SDL_Renderer* _renderer)
{
	//渲染充电桩的透明红色作用范围
	for (const SDL_Rect& _rect : stationRects)
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 255, 0, 0, 100 });
	//渲染车辆的透明蓝色作用范围
	for (const SDL_Rect& _rect : vehicleRects)
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 0, 0, 255, 100 });
}