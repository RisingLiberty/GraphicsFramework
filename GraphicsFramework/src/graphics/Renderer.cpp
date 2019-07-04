#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

void Renderer::SubmitObject(SceneObject* mesh)
{
	m_scene_objects.push_back(mesh);
}