#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <SDL.h>
#include "../Manager.hpp"
#include "../../Tilemap/Map.h"
#include "../../../Header/Manager/Concrete/VehicleSpawner.h"

//��Ϸ��������
class GameManager :public Manager<GameManager>
{
	friend class Manager<GameManager>;

private:
	SDL_Window* window = nullptr;              //�洢��Ϸ�����ڣ�������ʾ��Ϸ����
	SDL_Renderer* renderer = nullptr;          //�洢��Ⱦ������Ⱦ������ʹ��GPU���м���
	SDL_Event event;                           //���ڵ���SDL�¼������ݲ�ͬ���¼�����������Ӧ�ķ�Ӧ
	bool isQuit = false;                       //������Ϸ��ѭ���Ƿ����
	int fps = 120;                             //��Ҫά�ֵ���Ϸ֡�ʣ���ÿ��ˢ�µ�֡��

	VehicleSpawner vehicleSpawner;

public:
	int Run(int, char**);

private:
	GameManager();                   
	~GameManager();                  
	void InitAssert(bool, const char*);        //��ʼ�����ԣ����ڳ�ʼ���������ݳ�ʼ���������صĲ����ж��Ƿ��ʼ���ɹ�
	
	void OnInput();                            //��ѭ���ڼ�������¼���������Ӧ�ķ�Ӧ
	void OnUpdate(double);                     //��ѭ������֡������ݵĸ���
	void OnRender();                           //��ѭ������Ⱦ��ͼ�ľ������
};

#endif
