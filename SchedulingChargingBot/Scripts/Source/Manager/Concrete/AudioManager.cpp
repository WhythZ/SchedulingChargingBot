#include "../../../Header/Manager/Concrete/AudioManager.h"

AudioManager::AudioManager()
{
	//��ȡ��Ƶ��Դ
	soundPool = &ResourceManager::Instance()->GetSoundPool();
}

void AudioManager::PlaySFX(SoundResID _id)
{
	//��������Mix_Chunk*��Ч����һ����-1��ʾ������һ�����õ�Channel���в��ţ��ڶ�����ָ��Ҫ���ŵ���Ƶ����������0����ֻ����һ�Σ�-1����ѭ����
	Mix_PlayChannel(-1, soundPool->find(_id)->second, 0);
}