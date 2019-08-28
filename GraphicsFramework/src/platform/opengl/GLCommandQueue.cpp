#include "stdafx.h"

#include "GLCommandQueue.h"
#include "GLCommandList.h"

GLCommandQueue::GLCommandQueue(unsigned int maxNrOfFramesInFlight):
	CommandQueue(maxNrOfFramesInFlight)
{
	m_command_lists.resize(1);
	m_command_lists[0] = (std::make_unique<GLCommandList>());
}

GLCommandQueue::~GLCommandQueue()
{

}

GLCommandList* GLCommandQueue::GetCommandList() const
{
	return m_command_lists.front()->As<GLCommandList>();
}