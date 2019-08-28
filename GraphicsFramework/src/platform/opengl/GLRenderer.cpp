#include "stdafx.h"

#include <GL/glew.h>
#include <graphics/Mesh.h>
#include <graphics/Material.h>

#include <scenegraph/SceneObject.h>

#include "GLHelperMethods.h"
#include "GLRenderer.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLVertexArray.h"

#include "GLShaderProgram.h"
#include "GLVertexShader.h"
#include "GLFragmentShader.h"

#include "GLDrawIndexedCommand.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

GLRenderer::GLRenderer()
{
	GetGLCommandList()->SetClearColor(m_clear_color);
	ImGui_ImplOpenGL3_Init("#version 330");
}

GLRenderer::~GLRenderer()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void GLRenderer::Draw()
{
	for (SceneObject* object : m_scene_objects)
	{
		Mesh* mesh = object->GetMesh();
		Material* material = object->GetMaterial();

		material->Use();

		mesh->GetVertexArray()->Bind();
		mesh->GetIndices()->Bind();
		
		Format format = mesh->GetIndices()->GetFormat();
		unsigned int count = (GLsizei)mesh->GetIndices()->GetCount();
		Topology topology = mesh->GetIndices()->GetTopology();

		GetGLCommandList()->Push(std::make_unique<GLDrawIndexedCommand>(count, topology, format));
	}

	m_scene_objects.clear();
}

void GLRenderer::ClearAllBuffers()
{
	GetGLCommandList()->ClearColorBuffer();
	GetGLCommandList()->ClearDepthStencilBuffer();
}

void GLRenderer::ClearColorBuffer()
{
	GetGLCommandList()->ClearColorBuffer();
}

void GLRenderer::ClearDepthStencilBuffer()
{
	GetGLCommandList()->ClearDepthStencilBuffer();
}

void GLRenderer::ClearDepthBuffer()
{
	GetGLCommandList()->ClearDepthBuffer();
}

void GLRenderer::ClearStencilBuffer()
{
	GetGLCommandList()->ClearStencilBuffer();
}

void GLRenderer::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}