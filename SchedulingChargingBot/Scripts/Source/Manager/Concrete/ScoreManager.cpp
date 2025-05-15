#include "../../../Header/Manager/Concrete/ScoreManager.h"
#include <SDL.h>
#include<iostream>

//返回一个double值表示当前时间
double ScoreManager::score_timer()
{
    #pragma region TimeScore 
    // 静态变量，仅在第一次调用时初始化，之后保留上次调用的值
    static Uint64 _lastCounter = SDL_GetPerformanceCounter();
    static Uint64 _counterFreq = SDL_GetPerformanceFrequency();

    Uint64 _currentCounter = SDL_GetPerformanceCounter();

    double _delta = (double)(_currentCounter - _lastCounter) / _counterFreq;

    //std::cout << _delta << "s" << std::endl;//测试用

    return _delta;
    #pragma endregion
}

ScoreManager::ScoreManager()
{

}

ScoreManager::~ScoreManager()
{

}

