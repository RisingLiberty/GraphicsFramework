#include "stdafx.h"

#include "UnmapBufferCommand.h"

UnmapBufferCommand::UnmapBufferCommand(const ApiBufferWrapper* buffer) :
	m_buffer(buffer)
{

}

UnmapBufferCommand::~UnmapBufferCommand()
{

}