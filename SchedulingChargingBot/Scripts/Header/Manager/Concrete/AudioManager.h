#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <vector>
#include "../Manager.hpp"
#include "../Concrete/ResourceManager.h"

class AudioManager :public Manager<AudioManager>
{
	friend class Manager<AudioManager>;
	
private:
	const SoundPool* soundPool;   //“Ù–ß≥ÿ

public:
	void PlaySFX(SoundResID);     //≤•∑≈“Ù–ß

private:
	AudioManager();
	~AudioManager() = default;
};

#endif
