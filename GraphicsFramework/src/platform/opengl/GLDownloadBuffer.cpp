#include "stdafx.h"

#include "GLDownloadBuffer.h"
#include "GLVertexBuffer.h"
#include "GLHelperMethods.h"

GLDownloadBuffer::GLDownloadBuffer(unsigned int size):
	DownloadBuffer(size)
{

}

GLDownloadBuffer::~GLDownloadBuffer()
{

}

void GLDownloadBuffer::Download(const ApiBufferWrapper* buffer)
{
	const GLBufferWrapper* gl_buffer = buffer->As<GLBufferWrapper>();

	switch (gl_buffer->GetType())
	{
	case BufferType::VERTEX: 
	{
		void* src_data = GetGLCommandList()->MapVertexBuffer(m_size);
		memcpy(m_data, src_data, m_size);
		GetGLCommandList()->UnmapIndexBuffer();
	}
	case BufferType::INDEX: 
	{
		void* src_data = GetGLCommandList()->MapIndexBuffer(m_size);
		memcpy(m_data, src_data, m_size);
		GetGLCommandList()->UnmapIndexBuffer();
	}
	}
}