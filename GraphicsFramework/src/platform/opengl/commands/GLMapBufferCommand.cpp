#include "stdafx.h"

#include "GLMapBufferCommand.h"

#include "../GLHelperMethods.h"
#include "../GLBufferWrapper.h"

GLMapBufferCommand::GLMapBufferCommand(const ApiBufferWrapper* buffer, void** gpuAddress, unsigned int size):
	MapBufferCommand(buffer),
	m_size(size),
	m_gpu_address(gpuAddress)
{

}

GLMapBufferCommand::~GLMapBufferCommand()
{

}

void GLMapBufferCommand::Execute()
{
	unsigned int offset = 0;

	switch (m_buffer->As<GLBufferWrapper>()->GetType())
	{
	case BufferType::VERTEX:			*m_gpu_address = glMapBufferRange(GL_ARRAY_BUFFER, offset, m_size, m_buffer->GetAccess().ToOpenGL());
	case BufferType::INDEX:				*m_gpu_address = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, m_size, m_buffer->GetAccess().ToOpenGL());
	case BufferType::COPY_SOURCE:		*m_gpu_address = glMapBufferRange(GL_COPY_READ_BUFFER, offset, m_size, m_buffer->GetAccess().ToOpenGL());
	case BufferType::COPY_DESTINATION:	*m_gpu_address = glMapBufferRange(GL_COPY_WRITE_BUFFER, offset, m_size, m_buffer->GetAccess().ToOpenGL());
	}
}
