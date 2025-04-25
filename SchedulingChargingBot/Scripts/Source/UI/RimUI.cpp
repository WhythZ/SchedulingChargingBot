#include "../../Header/UI/RimUI.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

RimUI::RimUI()
{
	const Map& _map = SceneManager::Instance()->map;
	
	#pragma region Stations
	std::unordered_map<size_t, SDL_Point> _stations = _map.GetStationIdxPool();
	//ע����׮��������1��ʼ��0����Чֵ
	for (int i = 1; i < _stations.size(); i++)
	{
		SDL_Point _point = _stations.find(i)->second;
		
		//�����Ӧλ����Ƭ���ĵ�λ��
		int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
		int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
		//�Ը���ƬΪ���ģ���Χ3x3�ľ��η�Χ�ǿɴ�����Χ�����׮�������˳�硢�������Դ�������磩
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
	std::unordered_map<size_t, SDL_Point> _vehicles = _map.GetVehicleIdxPool();
	for (int i = 1; i < _vehicles.size(); i++)
	{
		SDL_Point _point = _vehicles.find(i)->second;

		//�����Ӧλ����Ƭ���ĵ�λ��
		int _x = _point.x * TILE_SIZE + TILE_SIZE / 2;
		int _y = _point.y * TILE_SIZE + TILE_SIZE / 2;
		//�Ը���ƬΪ���ģ���Χ3x3�ľ��η�Χ�ǿɴ�����Χ�����׮�������˳�硢�������Դ�������磩
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
	//��Ⱦ���׮��͸����ɫ���÷�Χ
	for (const SDL_Rect& _rect : stationRects)
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 255, 0, 0, 100 });
	//��Ⱦ������͸����ɫ���÷�Χ
	for (const SDL_Rect& _rect : vehicleRects)
		UIManager::Instance()->DrawBox(_renderer, { _rect.x, _rect.y }, { _rect.w, _rect.h }, { 0, 0, 255, 100 });
}