#include "stdafx.h"

#include "GLSetBufferDataCommand.h"
#include "GLHelperMethods.h"

#include "GLIndexBuffer.h"

GLSetBufferDataCommand::GLSetBufferDataCommand(ApiBufferWrapper* buffer, unsigned int size, const void* data, BufferUsage usage, BufferType type) :
	SetBufferDataCommand(buffer, size, data, usage),
	m_type(type)
{

}

GLSetBufferDataCommand::~GLSetBufferDataCommand()
{

}

void GLSetBufferDataCommand::Execute()
{
	switch (m_usage)
	{
	case EBufferUsage::STATIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GLCALL(glBufferStorage(GL_ARRAY_BUFFER, m_size, m_data, 0)); break;
		case BufferType::INDEX: GLCALL(glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, 0)); break;
		}
		break;
	case EBufferUsage::DYNAMIC:
		switch (m_type)
		{
		case BufferType::VERTEX: GLCALL(glBufferData(GL_ARRAY_BUFFER, m_size, m_data, m_usage.ToOpenGL())); break;
		case BufferType::INDEX: GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, m_usage.ToOpenGL())); break;
		}
		break;
	}
}