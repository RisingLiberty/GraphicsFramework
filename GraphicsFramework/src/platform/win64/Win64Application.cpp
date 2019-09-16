#include "stdafx.h"

#include "Win64Application.h"
#include "Win64Window.h"

#include "core/Timer.h"

#include "graphics/Context.h"
#include "graphics/Renderer.h"

#include "controllers/SceneController.h"

#include "Events/SwitchApiEvent.h"

namespace
{
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 800;
	const std::string& WINDOW_TITLE = "Sandbox";

	const float UPDATE_FREQUENCY = 60; //frames per seconds
	const float SECONDS_PER_UPDATE = 1.0f / UPDATE_FREQUENCY;

}

Win64Application::Win64Application(AreFramesCapped areFramesCapped) :
	m_are_frames_capped(areFramesCapped)
{
	m_window = std::make_unique<Win64Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	m_context.reset(Context::Create(API::DIRECTX12, m_window.get()));
}

Win64Application::~Win64Application()
{

};

void Win64Application::Run()
{
	m_is_exiting = false;
	while (!m_is_exiting)
	{
		this->Initialize();
		Context::GetCurrent()->PostInitialize();
		spdlog::info("Application is running");

		bool is_running = true;

		unsigned int frame_count = 0;
		float frame_time = 0;

		while (is_running)
		{
			using namespace std::chrono_literals;
			if (m_window->EventLoop() == WM_QUIT && m_switched_api_last_frame == false)
			{
				is_running = false;
				m_is_exiting = true;
				continue;
			}

			m_switched_api_last_frame = false;
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

			if (m_switched_api_last_frame)
			{
				system("cls");

				// Switching from opengl to vulkan currently leaves a opengl context existing in the window.
				// By recreating the window, this is forcfully destroyed.
				// (wglDeleteContext is not working for some reason)

				bool should_recreate_window = m_context_to_switch_to == API::VULKAN || Context::GetApi() == API::DIRECTX12;

				// std first sets new pointer, then deletes the old one.
				// we want the reverse, that's why reset is called twice
				m_context.reset();

				if (should_recreate_window)
				{
					m_window.reset();
					m_window = std::make_unique<Win64Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
					m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
				}

				//std::this_thread::sleep_for(5s);
				m_context.reset(Context::Switch(m_context_to_switch_to, m_window.get()));
				m_scene_controller->Clear();


				is_running = false;
				continue;
			}

			if (Context::GetCurrent()->GetRenderer())
				this->Draw();

			m_window->Present();

			//fix so draw updates are synced with display refresh rate
		}
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
	Context::GetCurrent()->GetRenderer()->End();
	Context::GetCurrent()->End();
	Context::GetCurrent()->GetRenderer()->RenderImgui();
}

void Win64Application::OnEvent(const Event& event)
{
	const SwitchApiEvent* switch_api_event = static_cast<const SwitchApiEvent*>(&event);

	m_switched_api_last_frame = true;
	m_context_to_switch_to = switch_api_event->GetApi();
}
