#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <functional>
#include <vector>
#include <SDL.h>
#include "../Infra/Timer.h"

//动画播放的默认帧间隔
#define ANIMATION_DEFAULT_FRAME_INTERVAL 0.1
//动画播放的较大帧间隔
#define ANIMATION_LARGER_FRAME_INTERVAL 0.2

class Animation
{
private:
	Timer timer;                                                //计时器用于帧动画更新
	std::function<void()> trigger;                              //动画播放结束后触发的回调函数
	bool isLoop = true;                                         //是否循环播放动画

	SDL_Texture* srcSpriteSheetTexture = nullptr;               //使用的源SpriteSheet图片
	std::vector<SDL_Rect> srcSpriteSheetRects;                  //裁剪SpriteSheet获取所需的逐帧图片

	size_t frameIdx = 0;                                        //动画播放到的帧的索引
	int frameWidth = 0, frameHeight = 0;                        //每帧动画纹理的宽高
	SDL_Texture* dstAnimTexture = nullptr;                      //每帧动画实际播放的纹理图片
	SDL_Rect* animRect = nullptr;                               //指定纹理图片渲染在窗口上的位置
	   
public:
	Animation();
	~Animation() = default;

	//设置动画素材；即以第四参数传入的索引列表截取传入源SpriteSheet（二三参数位定义了其宽高帧图片个数）中的若干矩形图片，以用作动画各帧
	void SetAnimFrames(SDL_Texture*, int, int, const std::vector<int>&);
	void SetOnAnimFinished(std::function<void()>);              //设置动画播放结束的回调函数
	void SetLoop(bool);                                         //设置动画是否循环播放
	//设置每帧动画播放的时间间隔，即定时器的waitTime
	void SetFrameInterval(double = ANIMATION_DEFAULT_FRAME_INTERVAL);

	//设置动画播放，即传入渲染器、动画被渲染的Rect左上顶点位置、动画的旋转角度
	void OnRender(SDL_Renderer*, const SDL_Point&, double = 0);
	void OnUpdate(double);                                      //动画的帧更新

	void Restart();                                             //重置动画播放
};

#endif