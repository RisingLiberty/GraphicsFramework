#pragma once

#include "Events/event.h"

using WindowEventCallback = std::function<void(const Event& event)>;

class Window
{
public:
	Window(unsigned int width, unsigned int height, bool isFullScreen = false, bool isVsync = false);
	virtual ~Window();

	virtual int EventLoop() = 0;
	virtual void* GetHandle() const = 0;

	void SetEventCallback(const WindowEventCallback& callback);

	bool IsPaused() const;

protected:
	struct Properties
	{
		unsigned int width;
		unsigned int height;
		bool is_fullscreen;
		bool vsync;
	};
	Properties m_properties;

	WindowEventCallback m_event_callback;
	bool m_is_paused;
	
};