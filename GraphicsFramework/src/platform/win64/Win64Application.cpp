#include "stdafx.h"

#include "Win64Application.h"
#include "Win64Window.h"

#include "core/Timer.h"

#include "graphics/Context.h"
#include "graphics/Renderer.h"

#include "controllers/SceneController.h"

#include "scenegraph/Scene.h"

#include "platform/directx12/Dx12HelperMethods.h"

namespace
{
	const int WINDOW_WIDTH = 720;
	const int WINDOW_HEIGHT = 480;
	const std::string& WINDOW_TITLE = "Sandbox";

	const float UPDATE_FREQUENCY = 60; //frames per seconds
	const float SECONDS_PER_UPDATE = 1.0f / UPDATE_FREQUENCY;

}

Win64Application::Win64Application(AreFramesCapped areFramesCapped):
	m_are_frames_capped(areFramesCapped)
{
	m_window = std::make_unique<Win64Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	m_context.reset(Context::Create(Context::API::VULKAN, m_window.get()));
}

Win64Application::~Win64Application()
{

};

void Win64Application::Run()
{
	Context::GetCurrent()->PostInitialize();

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
		}

		// sleep thread so window can activate again when debugging
		if (m_window->IsPaused())
		{
			std::this_thread::sleep_for(1s);
			continue;
		}

		m_timer->Tick();

		if (m_are_frames_capped == AreFramesCapped::YES)
		{
			frame_time += m_timer->GetDeltaTimeInSeconds();

			if (frame_time > SECONDS_PER_UPDATE)
			{
				this->Update(m_timer->GetDeltaTimeInSeconds());
				frame_time = 0;
				++frame_count;
			}
		}
		else
		{
			this->CalculateFrameStats();
			this->Update(m_timer->GetDeltaTimeInSeconds());
			++frame_count;
		}

		if (Context::GetCurrent()->GetRenderer())
			this->Draw();

		m_window->Present();

		//fix so draw updates are synced with display refresh rate
	}
}

void Win64Application::Update(float dTime)
{
	m_scene_controller->Update(dTime);
	
}

void Win64Application::Draw()
{
	Context::GetCurrent()->Begin();
	Context::GetCurrent()->Clear();
	Context::GetCurrent()->GetRenderer()->Begin();
	m_scene_controller->Draw();
	Context::GetCurrent()->GetRenderer()->Draw();
	Context::GetCurrent()->GetRenderer()->RenderImgui();
	Context::GetCurrent()->GetRenderer()->End();
	Context::GetCurrent()->End();
}

void Win64Application::OnEvent(const Event& event)
{

}
