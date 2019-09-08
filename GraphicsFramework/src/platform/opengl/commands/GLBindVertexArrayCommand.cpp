#include "stdafx.h"

#include "GLBindVertexArrayCommand.h"
#include "../GLVertexArray.h"

#include "../GLHelperMethods.h"

GLBindVertexArrayCommand::GLBindVertexArrayCommand(const VertexArray* va):
	BindVertexArrayCommand(va)
{

}

GLBindVertexArrayCommand::~GLBindVertexArrayCommand()
{

}

void GLBindVertexArrayCommand::Execute()
{
	GLCALL(glBindVertexArray(m_vertex_array->As<GLVertexArray>()->GetId()));
}