#include "stdafx.h"

#include "SetBufferDataCommand.h"

SetBufferDataCommand::SetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data):
	m_buffer(buffer),
	m_size(size),
	m_data(nullptr)
{
	m_data = malloc(size);

	if (data)
		memcpy(m_data, data, size);
	else
		ZeroMemory(m_data, size);
}

SetBufferDataCommand::~SetBufferDataCommand()
{
	free(m_data);
}

std::string SetBufferDataCommand::ToString() const
{
	return "Set buffer data";
}
