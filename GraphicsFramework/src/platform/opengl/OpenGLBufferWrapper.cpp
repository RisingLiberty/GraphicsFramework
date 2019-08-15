#include "stdafx.h"

#include "OpenGLBufferWrapper.h"
#include "OpenGLHelperMethods.h"
#include "OpenGLDownloadBuffer.h"

OpenGLBufferWrapper::OpenGLBufferWrapper(unsigned int size, BufferUsage usage, BufferType type, const void* data) :
	ApiBufferWrapper(usage),
	m_type(type)
{
	m_id = GetOpenGLCommandList()->CreateBuffer();

	if (data)
	{
		switch (type)
		{
		case BufferType::VERTEX: GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_id)); break;
		case BufferType::INDEX: GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id)); break;
		}
		this->SetDataInternal(data, size);
	}
}

OpenGLBufferWrapper::~OpenGLBufferWrapper()
{
	GetOpenGLCommandList()->DeleteBuffer(m_id);
}

BufferType OpenGLBufferWrapper::GetType() const
{
	return m_type;
}

void OpenGLBufferWrapper::SetDataInternal(const void* data, unsigned int size)
{
	switch (m_usage)
	{
	case EBufferUsage::DYNAMIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GetOpenGLCommandList()->SetVertexBufferDataDynamic(data, size, m_usage.ToOpenGL()); break;
		case BufferType::INDEX: GetOpenGLCommandList()->SetIndexBufferDataDynamic(data, size, m_usage.ToOpenGL()); break;
		}
	case EBufferUsage::STATIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GetOpenGLCommandList()->SetVertexBufferDataStatic(data, size); break;
		case BufferType::INDEX: GetOpenGLCommandList()->SetIndexBufferDataStatic(data, size); break;
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
