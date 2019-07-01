#include "stdafx.h"

#include "OpenGLIndexBuffer.h"

#include <GL/glew.h>
#include "HelperMethods.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(size_t count):
	IndexBuffer(count)
{
	GLCALL(glGenBuffers(1, &m_id));
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_id));
}

void OpenGLIndexBuffer::SetData(void* data)
{
	this->Bind();
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void OpenGLIndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void OpenGLIndexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}