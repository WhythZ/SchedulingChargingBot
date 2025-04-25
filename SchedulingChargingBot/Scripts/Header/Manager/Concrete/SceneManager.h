#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../Manager.hpp"
#include "../../Tilemap/Map.h"

class SceneManager :public Manager<SceneManager>
{
	friend class Manager<SceneManager>;

public:
	Map map;                                     //�洢�����ص���Ƭ��ͼ
	SDL_Rect mapRect = { 0 };                    //���ڴ�Ŀ�괰�����и��һ��������������mapTexture�Ļ�������

	SDL_Point cursorPosition = { 0,0 };          //�洢���ָ��λ�ã���λ�����¼��������б�ˢ��

private:
	SDL_Texture* mapTexture = nullptr;           //�洢����Ⱦ��һ����SDL_Texture*����ͼƬ����Ƭ��ͼ

public:
	bool Init(SDL_Renderer*);

	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	SDL_Point GetCursorPosition() const;         //��ȡ���λ��
	SDL_Point GetCursorTileIdx() const;          //��ȡ���λ�����ڵ���Ƭ����

private:
	SceneManager() = default;
	~SceneManager() = default;

	bool GenerateTilemapTexture(SDL_Renderer*);  //���ɳ�����ͼ����
};

#endif
