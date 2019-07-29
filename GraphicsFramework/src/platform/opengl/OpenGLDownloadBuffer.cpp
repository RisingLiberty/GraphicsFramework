#include "stdafx.h"

#include "OpenGLDownloadBuffer.h"
#include "OpenGLVertexBuffer.h"

OpenGLDownloadBuffer::OpenGLDownloadBuffer()
{

}

OpenGLDownloadBuffer::~OpenGLDownloadBuffer()
{

}

void OpenGLDownloadBuffer::Download(const VertexBuffer* vb)
{
	if (m_data)
		return;

	static_cast<const OpenGLVertexBuffer*>(vb)->Bind();
	m_data = malloc(vb->GetSize());
	unsigned int offset = 0;
	void* src_data = glMapBufferRange(GL_ARRAY_BUFFER, offset, vb->GetSize(), GL_MAP_READ_BIT);
	memcpy(m_data, src_data, vb->GetSize());
	glUnmapBuffer(GL_ARRAY_BUFFER);
}