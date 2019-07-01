#include "stdafx.h"

#include "Timer.h"

Timer::Timer()
{
	m_is_stopped = false;
	m_start = HighResClock::now();
}

Timer::~Timer() = default;

void Timer::Reset()
{
	m_start = HighResClock::now();
}

void Timer::Start()
{
	//Accumulate the time elapsed betwee stop and start pairs.
	//
	//						|<--------d--------->|
	//------*---------------*--------------------*-------------> time
	//  m_BaseTime      m_StopTime            startTime

	TimePoint curr_time = HighResClock::now();
	if (m_is_stopped)
	{
		m_paused_time += (curr_time - m_stop_time).count();

		m_prev_time_point = HighResClock::now();
		m_is_stopped = false;
	}

}

void Timer::Stop()
{
	if (!m_is_stopped)
	{
		m_stop_time = HighResClock::now();
		m_is_stopped = true;
	}
}

void Timer::Tick()
{
	if (m_is_stopped)
	{
		m_delta_time = 0;
		return;
	}

	TimePoint curr_time = HighResClock::now();

	//Time difference between this frame and the previous
	using ms = std::chrono::duration<float, std::milli>;
	m_delta_time = std::chrono::duration_cast<ms>(curr_time - m_prev_time_point).count();

	//Prepare for next frame
	m_prev_time_point = curr_time;

	//Force nonnegative. The DXSDK's CDXUTTimer mentions that if the processor goes into a power save mode
	//or we get shuffled to another processor, then m_DeltaTime can be negative.
	if (m_delta_time < 0)
		m_delta_time = 0;

}

float Timer::GetEllapsedMilliseconds() const
{
	return std::chrono::duration_cast<milliseconds>(HighResClock::now() - m_start).count();
}

float Timer::GetEllapsedSeconds() const
{
	return std::chrono::duration_cast<milliseconds>(HighResClock::now() - m_start).count() * 0.001f;
}


float Timer::GetDeltaTime() const
{
	return m_delta_time;
}

float Timer::GetDeltaTimeInSeconds() const
{
	return (float)m_delta_time * 0.001f;
}

float Timer::GetPausedTime() const
{
	return m_paused_time;
}