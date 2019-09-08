#include "stdafx.h"

#include "GLClearDepthBufferCommand.h"
#include "../GLHelperMethods.h"

GLClearDepthBufferCommand::GLClearDepthBufferCommand(float depthValue):
	ClearDepthBufferCommand(depthValue)
{

}

GLClearDepthBufferCommand::~GLClearDepthBufferCommand()
{

}

void GLClearDepthBufferCommand::Execute()
{
	GLCALL(glClearDepth(m_depth_value));
	GLCALL(glClear(GL_DEPTH_BUFFER_BIT));
}