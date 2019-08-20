#include "stdafx.h"

#include "OpenGLCommandQueue.h"
#include "OpenGLCommandList.h"

OpenGLCommandQueue::OpenGLCommandQueue(unsigned int maxNrOfFramesInFlight):
	CommandQueue(maxNrOfFramesInFlight)
{
	m_command_lists.push_back(std::make_unique<OpenGLCommandList>());
}

OpenGLCommandQueue::~OpenGLCommandQueue()
{

}

OpenGLCommandList* OpenGLCommandQueue::GetCommandList() const
{
	return m_command_lists.front()->As<OpenGLCommandList>();
}