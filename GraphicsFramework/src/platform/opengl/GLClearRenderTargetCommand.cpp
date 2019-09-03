#include "stdafx.h"

#include "GLClearRenderTargetCommand.h"
#include "GLHelperMethods.h"

GLClearRenderTargetCommand::GLClearRenderTargetCommand(const std::array<float, 4>& clearColor):
	ClearRenderTargetCommand(clearColor)
{

}

GLClearRenderTargetCommand::~GLClearRenderTargetCommand()
{

}

void GLClearRenderTargetCommand::Execute()
{
	GLCALL(glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}