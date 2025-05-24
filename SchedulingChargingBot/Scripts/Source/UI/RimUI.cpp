#include "../../Header/UI/RimUI.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"
#include "../../Header/Manager/Concrete/ChargeableManager.h"

#include <iostream>

void RimUI::OnRender(SDL_Renderer* _renderer)
{
	static const Map& _map = SceneManager::Instance()->map;
	std::map<size_t, SDL_Rect> _stationRects = _map.GetStationRects();
	const std::vector<Vehicle*>& _vehicleList = ChargeableManager::Instance()->GetVehicleList();

	//渲染充电桩的透明红色作用范围
	for (const auto& _iter : _stationRects)
	{
		const SDL_Rect& _rect = _iter.second;
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 255, 0, 0, 100 });
	}
	//渲染车辆的透明蓝色作用范围
	for (const Vehicle* _vehicle : _vehicleList)
	{
		const SDL_Rect& _rect = _vehicle->chargedRect;
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, _vehicle->rimColor);
	}
}