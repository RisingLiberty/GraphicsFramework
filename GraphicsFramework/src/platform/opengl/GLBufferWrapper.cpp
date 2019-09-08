#include "stdafx.h"

#include "GLBufferWrapper.h"
#include "GLHelperMethods.h"
#include "GLDownloadBuffer.h"

#include "GLDirectCommandList.h"
#include "GLCreateBufferCommand.h"
#include "GLBindIndexBufferCommand.h"
#include "GLSetBufferDataCommand.h"
#include "GLDeleteBufferCommand.h"

GLBufferWrapper::GLBufferWrapper(unsigned int size, BufferUsage usage, BufferAccess access, BufferType type, const void* data) :
	ApiBufferWrapper(usage, access),
	m_type(type)
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLCreateBufferCommand>(&m_id));
}

GLBufferWrapper::~GLBufferWrapper()
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLDeleteBufferCommand>(m_id));
}

BufferType GLBufferWrapper::GetType() const
{
	return m_type;
}

unsigned int GLBufferWrapper::GetId() const
{
	return m_id;
}

void GLBufferWrapper::SetDataInternal(const void* data, unsigned int size)
{
	GetGLCommandList()->Push(std::make_unique<GLSetBufferDataCommand>(this, size, data, m_type));
}

std::unique_ptr<DownloadBuffer> GLBufferWrapper::DownloadDataToBuffer(unsigned int size) const
{
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<GLDownloadBuffer>(size);
	buffer->Download(this);
	return std::move(buffer);
}
