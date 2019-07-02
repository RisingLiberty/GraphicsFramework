#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

void Renderer::SubmitMesh(SceneObject* mesh)
{
	m_scene_objects.push_back(mesh);
}