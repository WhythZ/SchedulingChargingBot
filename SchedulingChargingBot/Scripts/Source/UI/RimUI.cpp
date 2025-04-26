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
	std::vector<SDL_Rect> _stationRects = _map.GetStationRects();
	std::vector<SDL_Rect> _vehicleRects = _map.GetVehicleRects();

	//��Ⱦ���׮��͸����ɫ���÷�Χ
	for (const SDL_Rect& _rect : _stationRects)
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 255, 0, 0, 100 });
	//��Ⱦ������͸����ɫ���÷�Χ
	for (const SDL_Rect& _rect : _vehicleRects)
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 0, 0, 255, 100 });
}