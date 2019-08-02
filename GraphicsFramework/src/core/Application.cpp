#include "stdafx.h"

#include "Application.h"
#include "Timer.h"

#include "graphics/window.h"
#include "graphics/Context.h"

#include "controllers/SceneController.h"

#include "scenegraph/Scene.h"


Application::Application()
{
	m_timer = std::make_unique<Timer>();
	m_scene_controller = std::make_unique<SceneController>();
};

Application::~Application() = default;

void Application::CalculateFrameStats() const
{
	// Calculates fps and time it takes to render 1 frame
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	const int SECONDS_PER_UPDATE = 10;

	frameCount++;

	// Update every second
	if ((m_timer->GetEllapsedSeconds() - timeElapsed) >= SECONDS_PER_UPDATE)
	{
		float fps = (float)frameCount / SECONDS_PER_UPDATE; //after 1 second, frameCount holds the number of frames that passed.
		float mspf = 1000.0f / fps;

		// make the logging fixed
		std::cout << std::fixed;

		spdlog::info("fps: {}", fps);
		spdlog::info("milisecondss per frame: {}", mspf);

		frameCount = 0;
		timeElapsed += SECONDS_PER_UPDATE;
	}
}

void Application::PushScene(std::unique_ptr<Scene> scene)
{
	m_scene_controller->Push(std::move(scene));
}