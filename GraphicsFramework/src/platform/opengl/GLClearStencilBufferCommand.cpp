#include "stdafx.h"

#include "GLClearStencilBufferCommand.h"
#include "GLHelperMethods.h"

GLClearStencilBufferCommand::GLClearStencilBufferCommand(unsigned int stencilValue) :
	ClearStencilBufferCommand(stencilValue)
{

}

GLClearStencilBufferCommand::~GLClearStencilBufferCommand()
{

}

void GLClearStencilBufferCommand::Execute()
{
	GLCALL(glClearStencil(m_stencil_value));
	GLCALL(glClear(GL_STENCIL_BUFFER_BIT));
}