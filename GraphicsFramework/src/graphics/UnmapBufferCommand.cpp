#include "stdafx.h"

#include "UnmapBufferCommand.h"

UnmapBufferCommand::UnmapBufferCommand(const ApiBufferWrapper* buffer) :
	m_buffer(buffer)
{

}

UnmapBufferCommand::~UnmapBufferCommand()
{

}

std::string UnmapBufferCommand::ToString() const
{
	return "Unmap buffer";
}
