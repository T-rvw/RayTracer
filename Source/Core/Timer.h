#pragma once

#include <inttypes.h>

class Timer final
{
public:	
	Timer() = default;
	~Timer() = default;
	
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	void init();
	float tick();

private:
	uint64_t m_lastCounter;
};