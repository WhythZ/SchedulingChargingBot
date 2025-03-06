#ifndef _TILE_H_
#define _TILE_H_

#include <vector>

//�궨��ÿ��������Ƭ�ı߳�����λΪ���أ�
#define TILE_SIZE 64

struct Tile
{
	#pragma region LoadFromCSV
	int tilesetIdx = 0;                      //ȷ������Ƭ����Ƭ������ѡȡ��һ��ͼƬ
	int functionFlag = 0;                    //0��ʾ��Ƭ�޹��ܣ�����1��ʼ��ʾ�������ɵ㣬����-1��ʼ��ʾ���׮
	#pragma endregion
};

//�Զ����ά��Ƭ������
typedef std::vector<std::vector<Tile>> TileMap;

#endif