#include "../../../Header/Manager/Concrete/AudioManager.h"

AudioManager::AudioManager()
{
	//获取音频资源
	soundPool = &ResourceManager::Instance()->GetSoundPool();
}

void AudioManager::PlaySFX(SoundResID _id)
{
	//混音播放Mix_Chunk*音效，第一参数-1表示任意找一个可用的Channel进行播放，第二参数指代要播放的音频，第三参数0代表只播放一次（-1无限循环）
	Mix_PlayChannel(-1, soundPool->find(_id)->second, 0);
}