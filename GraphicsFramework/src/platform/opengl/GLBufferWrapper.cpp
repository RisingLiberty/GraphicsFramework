#include "stdafx.h"

#include "GLBufferWrapper.h"
#include "GLHelperMethods.h"
#include "GLDownloadBuffer.h"

#include "GLCreateBufferCommand.h"
#include "GLBindIndexBufferCommand.h"
#include "GLSetBufferDataCommand.h"
#include "GLDeleteBufferCommand.h"

GLBufferWrapper::GLBufferWrapper(unsigned int size, BufferUsage usage, BufferType type, const void* data) :
	ApiBufferWrapper(usage),
	m_type(type)
{
	GetGLCommandList()->Push(std::make_unique<GLCreateBufferCommand>(&m_id));
	//m_id = GetGLCommandList()->CreateBuffer();
}

GLBufferWrapper::~GLBufferWrapper()
{
	GetGLCommandList()->Push(std::make_unique<GLDeleteBufferCommand>(m_id));
	//GetGLCommandList()->DeleteBuffer(m_id);
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
	GetGLCommandList()->Push(std::make_unique<GLSetBufferDataCommand>(this, size, data, m_usage, m_type));
}

std::unique_ptr<DownloadBuffer> GLBufferWrapper::DownloadDataToBuffer(unsigned int size) const
{
	this->GLBind();
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<GLDownloadBuffer>(size);
	buffer->Download(this);
	return std::move(buffer);
}
