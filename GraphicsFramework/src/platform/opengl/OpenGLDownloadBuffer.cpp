#include "stdafx.h"

#include "OpenGLDownloadBuffer.h"
#include "OpenGLVertexBuffer.h"

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

	m_data = malloc(m_size);
	unsigned int offset = 0;
	void* src_data = glMapBufferRange(GL_ARRAY_BUFFER, offset, m_size, GL_MAP_READ_BIT);
	memcpy(m_data, src_data, m_size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}