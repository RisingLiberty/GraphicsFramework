#include "stdafx.h"

#include <GL/glew.h>
#include <graphics/Mesh.h>
#include <graphics/Material.h>

#include <scenegraph/SceneObject.h>

#include "OpenGLHelperMethods.h"
#include "OpenGLRenderer.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexArray.h"

#include "OpenGLShaderProgram.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

OpenGLRenderer::OpenGLRenderer()
{
	GetOpenGLCommandList()->SetClearColor(m_clear_color);
	ImGui_ImplOpenGL3_Init("#version 330");
}

OpenGLRenderer::~OpenGLRenderer()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void OpenGLRenderer::Draw()
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

		unsigned int* offset = NULL_OFFSET;

		GetOpenGLCommandList()->DrawElements(topology.ToOpenGL(), count, format.ToOpenGL());
	}

	m_scene_objects.clear();
}

void OpenGLRenderer::ClearAllBuffers()
{
	GetOpenGLCommandList()->ClearColorBuffer();
	GetOpenGLCommandList()->ClearDepthStencilBuffer();
}

void OpenGLRenderer::ClearColorBuffer()
{
	GetOpenGLCommandList()->ClearColorBuffer();
}

void OpenGLRenderer::ClearDepthStencilBuffer()
{
	GetOpenGLCommandList()->ClearDepthStencilBuffer();
}

void OpenGLRenderer::ClearDepthBuffer()
{
	GetOpenGLCommandList()->ClearDepthBuffer();
}

void OpenGLRenderer::ClearStencilBuffer()
{
	GetOpenGLCommandList()->ClearStencilBuffer();
}

void OpenGLRenderer::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}