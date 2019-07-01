#include "stdafx.h"

#include "Win64Application.h"
#include "Win64Window.h"

#include "Core/Timer.h"

#include "graphics/Context.h"

namespace
{
	const int WINDOW_WIDTH = 720;
	const int WINDOW_HEIGHT = 480;
	const std::wstring& WINDOW_TITLE = L"Sandbox";
}

Win64Application::Win64Application()
{
	m_window = std::make_unique<Win64Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	m_timer = std::make_unique<Timer>();
	Context::Create(Context::API::OpenGL, m_window->GetHandle());
}

Win64Application::~Win64Application() = default;

void Win64Application::Run()
{
	spdlog::info("Application is running");

	bool is_running = true;

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

		this->CalculateFrameStats();
		this->Update(m_timer->GetDeltaTimeInSeconds());
		this->Draw();
	}

	std::cin.get();
}

void Win64Application::Update(float dTime)
{

}

void Win64Application::Draw()
{

}

void Win64Application::OnEvent(const Event& event)
{

}