#pragma once

#include "Events/event.h"

using WindowEventCallback = std::function<void(const Event& event)>;

class Window
{
public:
	struct Properties
	{
		unsigned int width;
		unsigned int height;
		bool is_fullscreen;
		bool vsync;
	};

	Window(unsigned int width, unsigned int height, bool isFullScreen = false, bool isVsync = true);
	virtual ~Window();

	virtual int EventLoop() = 0;
	virtual void* GetHandle() const = 0;
	virtual void Present() = 0;

	void SetEventCallback(const WindowEventCallback& callback);

	Properties GetPropeties() const;
	bool IsPaused() const;

protected:
	Properties m_properties;

	WindowEventCallback m_event_callback;
	bool m_is_paused;
	
};