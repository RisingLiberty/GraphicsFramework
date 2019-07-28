#include "stdafx.h"

#include "OpenGLDownloadBuffer.h"

OpenGLDownloadBuffer::OpenGLDownloadBuffer()
{

}

OpenGLDownloadBuffer::~OpenGLDownloadBuffer()
{

}

void OpenGLDownloadBuffer::Download(unsigned int size)
{
	m_data = malloc(size);
	unsigned int offset = 0;
	void* src_data = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);
	memcpy(m_data, src_data, size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}