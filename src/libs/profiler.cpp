#include "profiler.hpp"
#include "logger.hpp"
#include <chrono>

Timer::Timer(const char *name) : _name(name), _stopped(false) {
	_startTime = std::chrono::high_resolution_clock::now();	
}

Timer::~Timer() {
	if (!_stopped)
		Stop();
}

void Timer::Stop() {
	using namespace std::chrono;
	auto endTime = high_resolution_clock::now();

	auto start = time_point_cast<microseconds>(_startTime).time_since_epoch().count();
	auto end = time_point_cast<microseconds>(endTime).time_since_epoch().count(); 
	auto duration = end - start;

	Logger::LogAdvanced("%s took %dus(%fms)\n", _name, duration, (double)duration/1000);
	_stopped = true;
}
