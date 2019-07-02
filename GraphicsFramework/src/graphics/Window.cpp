#include "stdafx.h"

#include "Window.h"

Window::Window(unsigned int width, unsigned int height, bool isFullScreen, bool isVSync):
	m_is_paused(false)
{
	m_properties.width = width;
	m_properties.height = height;
	m_properties.is_fullscreen = isFullScreen;
	m_properties.vsync = isVSync;
}

Window::~Window() = default;

void Window::SetEventCallback(const WindowEventCallback& callback)
{
	m_event_callback = callback;
}

Window::Properties Window::GetPropeties() const
{
	return m_properties;
}

bool Window::IsPaused() const
{
	return m_is_paused;
}