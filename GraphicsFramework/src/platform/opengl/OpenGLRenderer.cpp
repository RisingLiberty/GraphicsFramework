#include "stdafx.h"

#include <GL/glew.h>
#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer()
{

}

OpenGLRenderer::~OpenGLRenderer()
{

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
