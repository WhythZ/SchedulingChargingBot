#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include <unordered_map>
#include <string>
#include "Tile.h"

class Map
{
private:
	TileMap tileMap;                                       //��¼��Ƭ��ͼ
	std::unordered_map<size_t, SDL_Point> stationIdxPool;  //��¼���׮����
	std::unordered_map<size_t, SDL_Point> vehicleIdxPool;  //��¼�������ɵ�����

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
	std::string TrimString(const std::string);             //������Ƭ�ַ����Ա�֤��ʽ��ͳһ�Ա��ȡ
	void LoadTileFromString(const std::string, Tile&);     //����Ƭ�ַ�������ȡ��Ϣ������
	void GenerateMapCache();                               //���ڶ�ȡ���ɵ�ͼ�Ļ���
};

#endif