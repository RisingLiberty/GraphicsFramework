#pragma once

class Window;
class Timer;
class SceneController;
class Scene;
class Context;

#include "Events/Event.h"

class Application
{
public:
	Application();
	virtual ~Application();

	virtual void OnEvent(const Event& event) = 0;
	virtual void Run() = 0;

protected:
	void CalculateFrameStats() const;
	void PushScene(std::unique_ptr<Scene> scene);
	
	virtual void Update(float dTime) = 0;
	virtual void Draw() = 0;

protected:
	std::unique_ptr<Window> m_window;
	std::unique_ptr<Timer> m_timer;
	std::unique_ptr<SceneController> m_scene_controller;
	std::unique_ptr<Context> m_context;
};