#include "stdafx.h"

#include "GLDeleteVertexArrayCommand.h"
#include "GLHelperMethods.h"

GLDeleteVertexArrayCommand::GLDeleteVertexArrayCommand(unsigned int* id) :
	m_id(id)
{

}

GLDeleteVertexArrayCommand::~GLDeleteVertexArrayCommand()
{

}

std::string GLDeleteVertexArrayCommand::ToString() const
{
	return "delete vertex array";
}

void GLDeleteVertexArrayCommand::Execute()
{
	GLCALL(glDeleteVertexArrays(1, m_id));
}