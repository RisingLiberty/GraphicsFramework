#include "stdafx.h"

#include "GLDrawIndexedCommand.h"
#include "../GLHelperMethods.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

GLDrawIndexedCommand::GLDrawIndexedCommand(unsigned int count, const Topology topology, const Format format):
	DrawIndexedCommand(count),
	m_topology(topology),
	m_format(format)
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