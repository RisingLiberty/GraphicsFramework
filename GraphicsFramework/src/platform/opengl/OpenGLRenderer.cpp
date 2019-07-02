#include "stdafx.h"

#include <GL/glew.h>
#include <graphics/Mesh.h>
#include <graphics/Material.h>

#include <scenegraph/SceneObject.h>

#include "HelperMethods.h"
#include "OpenGLRenderer.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexArray.h"

#include "OpenGLShaderProgram.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"

#include <graphics/MeshFactory.h>

OpenGLRenderer::OpenGLRenderer()
{
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

OpenGLRenderer::~OpenGLRenderer()
{

}

void OpenGLRenderer::Present()
{
	for (SceneObject* object : m_scene_objects)
	{
		Mesh* mesh = object->GetMesh();
		Material* material = object->GetMaterial();

		float color[] = { 1.0f, 0.3f, 0.8f, 1.0f };
		material->SetParameter("u_Color", color, sizeof(color));
		material->Use();

		VertexArray* va = VertexArray::Create(mesh->GetVertices(), mesh->GetVertexLayout());
		mesh->GetIndices()->Bind();

		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
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

