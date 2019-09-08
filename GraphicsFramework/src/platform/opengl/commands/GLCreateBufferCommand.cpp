#include "stdafx.h"

#include "GLCreateBufferCommand.h"
#include "../GLHelperMethods.h"

GLCreateBufferCommand::GLCreateBufferCommand(unsigned int* id) :
	m_id(id)
{

}

GLCreateBufferCommand::~GLCreateBufferCommand()
{

}

void GLCreateBufferCommand::Execute()
{
	GLCALL(glGenBuffers(1, m_id));
}