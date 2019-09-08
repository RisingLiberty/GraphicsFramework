#include "stdafx.h"

#include "GLSetBufferDataCommand.h"
#include "../GLHelperMethods.h"

#include "../GLIndexBuffer.h"

GLSetBufferDataCommand::GLSetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data, BufferType type) :
	SetBufferDataCommand(buffer, size, data),
	m_type(type)
{

}

GLSetBufferDataCommand::~GLSetBufferDataCommand()
{

}

void GLSetBufferDataCommand::Execute()
{
	switch (m_buffer->GetUsage())
	{
	case EBufferUsage::STATIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GLCALL(glBufferStorage(GL_ARRAY_BUFFER, m_size, m_data, m_buffer->GetAccess().ToOpenGL())); break;
		case BufferType::INDEX: GLCALL(glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, m_buffer->GetAccess().ToOpenGL())); break;			
		case BufferType::COPY_SOURCE: GLCALL(glBufferStorage(GL_COPY_READ_BUFFER, m_size, m_data, m_buffer->GetAccess().ToOpenGL()));
		case BufferType::COPY_DESTINATION: GLCALL(glBufferStorage(GL_COPY_WRITE_BUFFER, m_size, m_data, m_buffer->GetAccess().ToOpenGL()));
		}
		break;
	case EBufferUsage::DYNAMIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GLCALL(glBufferData(GL_ARRAY_BUFFER, m_size, m_data, m_buffer->GetUsage().ToOpenGL())); break;
		case BufferType::INDEX: GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, m_buffer->GetUsage().ToOpenGL())); break;
		case BufferType::COPY_SOURCE: GLCALL(glBufferData(GL_COPY_READ_BUFFER, m_size, m_data, m_buffer->GetUsage().ToOpenGL())); break;
		case BufferType::COPY_DESTINATION: GLCALL(glBufferData(GL_COPY_WRITE_BUFFER, m_size, m_data, m_buffer->GetUsage().ToOpenGL())); break;
		}
		break;
	}
}