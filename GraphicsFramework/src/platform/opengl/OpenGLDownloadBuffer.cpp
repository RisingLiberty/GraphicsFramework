#include "stdafx.h"

#include "OpenGLDownloadBuffer.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLHelperMethods.h"

OpenGLDownloadBuffer::OpenGLDownloadBuffer(unsigned int size):
	DownloadBuffer(size)
{

}

OpenGLDownloadBuffer::~OpenGLDownloadBuffer()
{

}

void OpenGLDownloadBuffer::Download(const ApiBufferWrapper* buffer)
{
	if (m_data)
		return;

	const OpenGLBufferWrapper* gl_buffer = static_cast<const OpenGLBufferWrapper*>(buffer);

	switch (gl_buffer->GetType())
	{
	case BufferType::VERTEX: 
	{
		void* src_data = GetOpenGLCommandList()->MapVertexBuffer(m_size);
		memcpy(m_data, src_data, m_size);
		GetOpenGLCommandList()->UnmapIndexBuffer();
	}
	case BufferType::INDEX: 
	{
		void* src_data = GetOpenGLCommandList()->MapIndexBuffer(m_size);
		memcpy(m_data, src_data, m_size);
		GetOpenGLCommandList()->UnmapIndexBuffer();
	}
	}
}