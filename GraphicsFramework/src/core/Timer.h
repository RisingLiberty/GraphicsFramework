#pragma once

#include <chrono>

using HighResClock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<HighResClock>;
using milliseconds = std::chrono::duration<float, std::milli>;

class Timer
{
public:
	Timer();
	~Timer();

	void Reset();

	void Start();
	void Stop(); 
	void Tick(); 
	float GetEllapsedMilliseconds() const;
	float GetEllapsedSeconds() const;

	float GetDeltaTime() const;
	float GetDeltaTimeInSeconds() const;
	float GetPausedTime() const;
	

private:
	TimePoint m_start;
	TimePoint m_stop_time;
	TimePoint m_prev_time_point;

	float m_delta_time;
	float m_paused_time;
	
	bool m_is_stopped;

};