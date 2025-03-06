#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <functional>
#include <vector>
#include <SDL.h>
#include "../Infra/Timer.h"

//�������ŵ�Ĭ��֡���
#define ANIMATION_DEFAULT_FRAME_INTERVAL 0.1
//�������ŵĽϴ�֡���
#define ANIMATION_LARGER_FRAME_INTERVAL 0.2

class Animation
{
private:
	Timer timer;                                                //��ʱ������֡��������
	std::function<void()> trigger;                              //�������Ž����󴥷��Ļص�����
	bool isLoop = true;                                         //�Ƿ�ѭ�����Ŷ���

	SDL_Texture* srcSpriteSheetTexture = nullptr;               //ʹ�õ�ԴSpriteSheetͼƬ
	std::vector<SDL_Rect> srcSpriteSheetRects;                  //�ü�SpriteSheet��ȡ�������֡ͼƬ

	size_t frameIdx = 0;                                        //�������ŵ���֡������
	int frameWidth = 0, frameHeight = 0;                        //ÿ֡��������Ŀ��
	SDL_Texture* dstAnimTexture = nullptr;                      //ÿ֡����ʵ�ʲ��ŵ�����ͼƬ
	SDL_Rect* animRect = nullptr;                               //ָ������ͼƬ��Ⱦ�ڴ����ϵ�λ��
	   
public:
	Animation();
	~Animation() = default;

	//���ö����زģ����Ե��Ĳ�������������б��ȡ����ԴSpriteSheet����������λ����������֡ͼƬ�������е����ɾ���ͼƬ��������������֡
	void SetAnimFrames(SDL_Texture*, int, int, const std::vector<int>&);
	void SetOnAnimFinished(std::function<void()>);              //���ö������Ž����Ļص�����
	void SetLoop(bool);                                         //���ö����Ƿ�ѭ������
	//����ÿ֡�������ŵ�ʱ����������ʱ����waitTime
	void SetFrameInterval(double = ANIMATION_DEFAULT_FRAME_INTERVAL);

	//���ö������ţ���������Ⱦ������������Ⱦ��Rect���϶���λ�á���������ת�Ƕ�
	void OnRender(SDL_Renderer*, const SDL_Point&, double = 0);
	void OnUpdate(double);                                      //������֡����

	void Restart();                                             //���ö�������
};

#endif