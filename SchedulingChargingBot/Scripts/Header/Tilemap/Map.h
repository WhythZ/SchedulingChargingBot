#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include <map>
#include <string>
#include "Tile.h"

class Map
{
private:
	TileMap tileMap;                                       //��¼��Ƭ��ͼ

	std::map<size_t, SDL_Point> stationIdxPool;            //��¼���׮����
	std::map<size_t, SDL_Point> vehicleIdxPool;            //��¼�������ɵ�����

	std::map<size_t, SDL_Rect> stationRects;               //��¼�����׮���÷�Χ
	std::map<size_t, SDL_Rect> vehicleRects;               //��¼���������ɵ����÷�Χ

public:
	Map() = default;
	~Map() = default;

	bool Load(const std::string&);

	TileMap GetTileMap() const;
	const std::map<size_t, SDL_Point>& GetStationIdxPool() const;
	const std::map<size_t, SDL_Point>& GetVehicleIdxPool() const;
	size_t GetWidthTileNum() const;
	size_t GetHeightTileNum() const;
	const std::map<size_t, SDL_Rect>& GetStationRects() const;
	const std::map<size_t, SDL_Rect>& GetVehicleRects() const;

private:
	std::string TrimString(const std::string);             //������Ƭ�ַ����Ա�֤��ʽ��ͳһ�Ա��ȡ
	void LoadTileFromString(const std::string, Tile&);     //����Ƭ�ַ�������ȡ��Ϣ������
	bool GenerateMapCache();                               //���ڶ�ȡ���ɵ�ͼ�Ļ���
};

#endif