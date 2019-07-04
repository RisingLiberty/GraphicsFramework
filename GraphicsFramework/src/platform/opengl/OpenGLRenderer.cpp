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


OpenGLRenderer::OpenGLRenderer()
{
	glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
	ImGui_ImplOpenGL3_Init("#version 330");
}

OpenGLRenderer::~OpenGLRenderer()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void OpenGLRenderer::Present()
{
	for (SceneObject* object : m_scene_objects)
	{
		Mesh* mesh = object->GetMesh();
		Material* material = object->GetMaterial();

		material->Use();

		VertexArray* va = VertexArray::Create(mesh->GetVertices(), mesh->GetVertexLayout());
		mesh->GetIndices()->Bind();

		GLCALL(glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetIndices()->GetCount(), GL_UNSIGNED_INT, 0));
	}

	m_scene_objects.clear();
}

void OpenGLRenderer::ClearAllBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::ClearDepthStencilBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::ClearStencilBuffer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

