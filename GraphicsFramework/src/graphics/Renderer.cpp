#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer()
{
	m_clear_color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

Renderer::~Renderer() = default;

void Renderer::SubmitObject(SceneObject* mesh)
{
	m_scene_objects.push_back(mesh);
}

void Renderer::Begin()
{

}

void Renderer::End()
{

}