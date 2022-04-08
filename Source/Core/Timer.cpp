#include "Timer.h"

#include <windows.h>

void Timer::init()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    m_lastCounter = li.QuadPart;
}

float Timer::tick()
{
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    int64_t frequency = li.QuadPart;

    QueryPerformanceCounter(&li);
    int64_t currentCounter = li.QuadPart;
    float deltaTime = static_cast<float>(currentCounter - m_lastCounter) / static_cast<float>(frequency);
    m_lastCounter = currentCounter;

    return deltaTime;
}