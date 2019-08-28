#include "stdafx.h"

#include "GLDrawIndexedCommand.h"
#include "GLHelperMethods.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

GLDrawIndexedCommand::GLDrawIndexedCommand(unsigned int count, Topology topology, Format format):
	DrawIndexedCommand(count, topology, format)
{

}

GLDrawIndexedCommand::~GLDrawIndexedCommand()
{
}

void GLDrawIndexedCommand::Execute()
{
	unsigned int* offset = NULL_OFFSET;
	GLCALL(glDrawElements(m_topology.ToOpenGL(), m_count, m_format.ToOpenGL(), NULL_OFFSET));
}