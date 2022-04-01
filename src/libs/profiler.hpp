#pragma once
#include <chrono>

#define PROFILING 1 

#if PROFILING
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif 

class Timer {
public:
	Timer(const char *name);
	
	~Timer();

	void Stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
	const char *_name;
	bool _stopped;
};


