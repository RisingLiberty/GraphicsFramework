#include "stdafx.h"

#include "OpenGLBufferWrapper.h"
#include "OpenGLHelperMethods.h"
#include "OpenGLDownloadBuffer.h"

OpenGLBufferWrapper::OpenGLBufferWrapper(unsigned int size, BufferUsage usage, BufferType type, const void* data) :
	ApiBufferWrapper(usage),
	m_type(type)
{
	GLCALL(glGenBuffers(1, &m_id));

	if (data)
	{
		switch (type)
		{
		case BufferType::INDEX: GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
		case BufferType::VERTEX: GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		}
		this->SetDataInternal(data, size);
	}
}

OpenGLBufferWrapper::~OpenGLBufferWrapper()
{
	GLCALL(glDeleteBuffers(1, &m_id));
}

void OpenGLBufferWrapper::SetDataInternal(const void* data, unsigned int size)
{
	switch (m_usage)
	{
	case EBufferUsage::DYNAMIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, m_usage.ToOpenGL())); break;
		case BufferType::INDEX: GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, m_usage.ToOpenGL())); break;
		}
	case EBufferUsage::STATIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GLCALL(glBufferStorage(GL_ARRAY_BUFFER, size, data, GL_MAP_READ_BIT)); break;
		case BufferType::INDEX: GLCALL(glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_MAP_READ_BIT)); break;
		}
	}
}

std::unique_ptr<DownloadBuffer> OpenGLBufferWrapper::DownloadDataToBuffer(unsigned int size) const
{
	this->GLBind();
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<OpenGLDownloadBuffer>(size);
	buffer->Download(this);
	return std::move(buffer);
}
