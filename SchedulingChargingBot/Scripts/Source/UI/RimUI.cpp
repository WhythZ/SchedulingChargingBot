#include "../../Header/UI/RimUI.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

#include <iostream>

//void RimUI::OnUpdate(SDL_Renderer* _renderer)
//{
//}

void RimUI::OnRender(SDL_Renderer* _renderer)
{
	static const Map& _map = SceneManager::Instance()->map;
	std::map<size_t, SDL_Rect> _stationRects = _map.GetStationRects();
	std::map<size_t, SDL_Rect> _vehicleRects = _map.GetVehicleRects();

	//渲染充电桩的透明红色作用范围
	for (const auto& _iter : _stationRects)
	{
		const SDL_Rect& _rect = _iter.second;
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 255, 0, 0, 100 });
	}
	//渲染车辆的透明蓝色作用范围
	for (const auto& _iter : _vehicleRects)
	{
		const SDL_Rect& _rect = _iter.second;
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 0, 0, 255, 100 });
	}
}