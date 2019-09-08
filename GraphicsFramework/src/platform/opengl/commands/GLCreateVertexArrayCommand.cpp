#include "stdafx.h"

#include "GLCreateVertexArrayCommand.h"
#include "../GLHelperMethods.h"

GLCreateVertexArrayCommand::GLCreateVertexArrayCommand(unsigned int* id) :
	m_id(id)
{

}

GLCreateVertexArrayCommand::~GLCreateVertexArrayCommand()
{

}

std::string GLCreateVertexArrayCommand::ToString() const
{
	return "Create vertex array";
}

void GLCreateVertexArrayCommand::Execute()
{
	GLCALL(glGenVertexArrays(1, m_id));
}