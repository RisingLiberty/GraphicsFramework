#include "stdafx.h"

#include "Application.h"
#include "graphics/window.h"
#include "Timer.h"

namespace
{
	const float UPDATE_FREQUENCY = 60;
	const float SECONDS_PER_UPDATE = 1.0f / UPDATE_FREQUENCY;
}

Application::Application()
{
};

Application::~Application() = default;

void Application::CalculateFrameStats() const
{
	// Calculates fps and time it takes to render 1 frame
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	// Update every second
	if ((m_timer->GetEllapsedSeconds() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCount; //after 1 second, frameCount holds the number of frames that passed.
		float mspf = 1000.0f / fps;

		// make the logging fixed
		std::cout << std::fixed;

		spdlog::info("fps: {}", fps);
		spdlog::info("milisecondss per frame: {}", mspf);

		frameCount = 0;
		++timeElapsed;
	}
}