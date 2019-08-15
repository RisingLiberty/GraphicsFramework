#include "stdafx.h"

#include "OpenGLCommandQueue.h"
#include "OpenGLCommandList.h"

OpenGLCommandQueue::OpenGLCommandQueue()
{
	m_command_lists.push_back(std::make_unique<OpenGLCommandList>());
}

OpenGLCommandQueue::~OpenGLCommandQueue()
{

}

OpenGLCommandList* OpenGLCommandQueue::GetCommandList() const
{
	return m_command_lists.front().get();
}