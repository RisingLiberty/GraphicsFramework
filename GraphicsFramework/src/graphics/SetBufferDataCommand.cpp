#include "stdafx.h"

#include "SetBufferDataCommand.h"

SetBufferDataCommand::SetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data, BufferUsage usage) :
	m_buffer(buffer),
	m_size(size),
	m_data(nullptr),
	m_usage(usage)
{
	m_data = malloc(size);
	memcpy(m_data, data, size);
}

SetBufferDataCommand::~SetBufferDataCommand()
{
	free(m_data);
}