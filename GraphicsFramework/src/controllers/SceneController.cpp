#include "stdafx.h"

#include "SceneController.h"

#include "scenegraph/Scene.h"

SceneController::SceneController() :
	m_active_scene(nullptr)
{
}

SceneController::~SceneController() = default;

void SceneController::Push(std::unique_ptr<Scene> scene)
{
	if (m_scenes.empty())
		m_active_scene = scene.get();

	m_scenes.push_back(std::move(scene));
}

void SceneController::Update(float dTime)
{
	spdlog::info("scene {} updating", m_active_scene->GetName());
	m_active_scene->Update(dTime);
}

void SceneController::Draw()
{
	spdlog::info("scene {} drawing", m_active_scene->GetName());
	m_active_scene->Draw();
}