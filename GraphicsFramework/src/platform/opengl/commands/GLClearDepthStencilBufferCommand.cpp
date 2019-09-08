#include "stdafx.h"

#include "GLClearDepthStencilBufferCommand.h"
#include "../GLHelperMethods.h"

GLClearDepthStencilBufferCommand::GLClearDepthStencilBufferCommand(float depthValue, unsigned int stencilValue) :
	ClearDepthStencilBufferCommand(depthValue, stencilValue)
{

}

GLClearDepthStencilBufferCommand::~GLClearDepthStencilBufferCommand()
{

}

void GLClearDepthStencilBufferCommand::Execute()
{
	GLCALL(glClearDepth(m_depth_value));
	GLCALL(glClearStencil(m_stencil_value));

	GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}