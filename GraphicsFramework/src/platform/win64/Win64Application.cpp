#include "stdafx.h"

#include "Win64Application.h"
#include "Win64Window.h"

#include "core/Timer.h"

#include "graphics/Context.h"

#include "controllers/SceneController.h"

#include "scenegraph/Scene.h"

namespace
{
	const int WINDOW_WIDTH = 720;
	const int WINDOW_HEIGHT = 480;
	const std::wstring& WINDOW_TITLE = L"Sandbox";

	const float UPDATE_FREQUENCY = 60; //frames per seconds
	const float SECONDS_PER_UPDATE = 1.0f / UPDATE_FREQUENCY;

}

Win64Application::Win64Application(AreFramesCapped areFramesCapped):
	m_are_frames_capped(areFramesCapped)
{
	m_window = std::make_unique<Win64Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	Context::Create(Context::API::OpenGL, m_window->GetHandle());
}

Win64Application::~Win64Application() = default;

void Win64Application::Run()
{
	spdlog::info("Application is running");

	bool is_running = true;

	unsigned int frame_count = 0;
	float frame_time = 0;

	while (is_running)
	{
		using namespace std::chrono_literals;
		if (m_window->EventLoop() == WM_QUIT)
		{
			is_running = false;
			false;
		}

		// sleep thread so window can activate again when debugging
		if (m_window->IsPaused())
		{
			std::this_thread::sleep_for(1s);
			continue;
		}

		m_timer->Tick();

		if (m_are_frames_capped == AreFramesCapped::Yes)
		{
			frame_time += m_timer->GetDeltaTimeInSeconds();

			if (frame_time > SECONDS_PER_UPDATE)
			{
				this->Update(m_timer->GetDeltaTimeInSeconds());
				this->Draw();
				frame_time = 0;
				++frame_count;
			}
		}
		else
		{
			this->CalculateFrameStats();
			this->Update(m_timer->GetDeltaTimeInSeconds());
			this->Draw();
			++frame_count;
		}
	}

	std::cin.get();
}

void Win64Application::Update(float dTime)
{
	m_scene_controller->Update(dTime);
	
}

void Win64Application::Draw()
{
	m_scene_controller->Draw();
}

void Win64Application::OnEvent(const Event& event)
{

}