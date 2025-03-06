#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <vector>
#include "../Manager.hpp"
#include "../Concrete/ResourceManager.h"

class AudioManager :public Manager<AudioManager>
{
	friend class Manager<AudioManager>;
	
private:
	const SoundPool* soundPool;   //��Ч��

public:
	void PlaySFX(SoundResID);     //������Ч

private:
	AudioManager();
	~AudioManager() = default;
};

#endif
