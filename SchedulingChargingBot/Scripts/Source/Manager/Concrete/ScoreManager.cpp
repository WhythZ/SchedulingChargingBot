#include "../../../Header/Manager/Concrete/ScoreManager.h"
#include <SDL.h>
#include<iostream>

void ScoreManager::OnUpdate(double _delta)
{
	levelTimer.OnUpdate(_delta);
}

void ScoreManager::RestartTimer()
{
	levelTimer.Restart();
}

double ScoreManager::GetPassTime() const
{
	return levelTimer.GetPassTime();
}