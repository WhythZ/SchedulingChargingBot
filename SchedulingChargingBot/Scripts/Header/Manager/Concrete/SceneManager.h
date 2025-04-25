#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../Manager.hpp"
#include "../../Tilemap/Map.h"

class SceneManager :public Manager<SceneManager>
{
	friend class Manager<SceneManager>;

public:
	Map map;                                     //存储被加载的瓦片地图
	SDL_Rect mapRect = { 0 };                    //用于从目标窗口中切割出一块区域，用于塞入mapTexture的画面内容

	SDL_Point cursorPosition = { 0,0 };          //存储鼠标指针位置，其位置在事件检测更新中被刷新

private:
	SDL_Texture* mapTexture = nullptr;           //存储被渲染成一整张SDL_Texture*纹理图片的瓦片地图

public:
	bool Init(SDL_Renderer*);

	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	SDL_Point GetCursorPosition() const;         //获取鼠标位置
	SDL_Point GetCursorTileIdx() const;          //获取鼠标位置所在的瓦片索引

private:
	SceneManager() = default;
	~SceneManager() = default;

	bool GenerateTilemapTexture(SDL_Renderer*);  //生成场景地图纹理
};

#endif
