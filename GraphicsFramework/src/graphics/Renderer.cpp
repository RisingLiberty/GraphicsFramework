#include "stdafx.h"

#include "Renderer.h"

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

void Renderer::SubmitMesh(Mesh* mesh)
{
	m_meshes_to_draw.push_back(mesh);
}