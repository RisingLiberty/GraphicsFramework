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

#include "commands/GLDrawIndexedCommand.h"

#include "commands/GLClearRenderTargetCommand.h"
#include "commands/GLClearDepthStencilBufferCommand.h"
#include "commands/GLClearDepthBufferCommand.h"
#include "commands/GLClearStencilBufferCommand.h"

#include "GLDirectCommandList.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

GLRenderer::GLRenderer()
{
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
	GetGLCommandList()->Push(std::make_unique<GLClearRenderTargetCommand>(m_clear_color));
	GetGLCommandList()->Push(std::make_unique<GLClearDepthStencilBufferCommand>(1.0f, 0));
}

void GLRenderer::ClearColorBuffer()
{
	GetGLCommandList()->Push(std::make_unique<GLClearRenderTargetCommand>(m_clear_color));
}

void GLRenderer::ClearDepthStencilBuffer()
{
	GetGLCommandList()->Push(std::make_unique<GLClearDepthStencilBufferCommand>(1.0f, 0));
}

void GLRenderer::ClearDepthBuffer()
{
	GetGLCommandList()->Push(std::make_unique<GLClearDepthBufferCommand>(1.0f));
}

void GLRenderer::ClearStencilBuffer()
{
	GetGLCommandList()->Push(std::make_unique<GLClearStencilBufferCommand>(0));
}

void GLRenderer::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}