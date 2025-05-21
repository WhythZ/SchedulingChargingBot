#include "../../../Header/Manager/Concrete/ScoreManager.h"
#include <SDL.h>
#include<iostream>

double ScoreManager::ScoreTimer()
{
    static Uint64 _lastCounter = SDL_GetPerformanceCounter();
    static Uint64 _counterFreq = SDL_GetPerformanceFrequency();

    Uint64 _currentCounter = SDL_GetPerformanceCounter();

    double _delta = (double)(_currentCounter - _lastCounter) / _counterFreq;

    return _delta;
}