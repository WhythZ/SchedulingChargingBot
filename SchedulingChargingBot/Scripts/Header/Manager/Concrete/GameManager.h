#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <SDL.h>
#include "../Manager.hpp"

//��Ϸ��������
class GameManager :public Manager<GameManager>
{
	friend class Manager<GameManager>;

private:
	SDL_Window* window = nullptr;              //�洢��Ϸ�����ڣ�������ʾ��Ϸ����
	SDL_Renderer* renderer = nullptr;          //�洢��Ⱦ������Ⱦ������ʹ��GPU���м���
	SDL_Event event;                           //���ڵ���SDL�¼������ݲ�ͬ���¼�����������Ӧ�ķ�Ӧ
	bool isQuit = false;                       //������Ϸ��ѭ���Ƿ����
	int fps = 60;                              //��Ҫά�ֵ���Ϸ֡�ʣ���ÿ��ˢ�µ�֡��
	
	SDL_Rect mapRect = { 0 };                  //���ڴ�Ŀ�괰�����и��һ��������������mapTexture�Ļ�������
	SDL_Texture* mapTexture = nullptr;         //�洢����Ⱦ��һ����SDL_Texture*����ͼƬ����Ƭ��ͼ
	
	SDL_Point cursorPosition = { 0,0 };        //�洢���ָ��λ�ã���λ�����¼��������б�ˢ��

public:
	int Run(int, char**);

	SDL_Rect GetMapRect() const;

private:
	GameManager();                   
	~GameManager();                  
	void InitAssert(bool, const char*);        //��ʼ�����ԣ����ڳ�ʼ���������ݳ�ʼ���������صĲ����ж��Ƿ��ʼ���ɹ�
	
	void OnInput();                            //��ѭ���ڼ�������¼���������Ӧ�ķ�Ӧ
	void OnUpdate(double);                     //��ѭ������֡������ݵĸ���
	void OnRender();                           //��ѭ������Ⱦ��ͼ�ľ������
	
	bool GenerateSceneTexture();               //���ɳ�����ͼ����
};

#endif
