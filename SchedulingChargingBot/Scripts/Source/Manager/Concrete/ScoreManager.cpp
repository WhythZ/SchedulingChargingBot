#include "../../../Header/Manager/Concrete/ScoreManager.h"
#include <SDL.h>
#include<iostream>

//����һ��doubleֵ��ʾ��ǰʱ��
double ScoreManager::score_timer()
{
    #pragma region TimeScore 
    // ��̬���������ڵ�һ�ε���ʱ��ʼ����֮�����ϴε��õ�ֵ
    static Uint64 _lastCounter = SDL_GetPerformanceCounter();
    static Uint64 _counterFreq = SDL_GetPerformanceFrequency();

    Uint64 _currentCounter = SDL_GetPerformanceCounter();

    double _delta = (double)(_currentCounter - _lastCounter) / _counterFreq;

    //std::cout << _delta << "s" << std::endl;//������

    return _delta;
    #pragma endregion
}

ScoreManager::ScoreManager()
{

}

ScoreManager::~ScoreManager()
{

}

