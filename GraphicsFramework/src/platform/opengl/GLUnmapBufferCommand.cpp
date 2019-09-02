#include "stdafx.h"

#include "GLUnmapBufferCommand.h"
#include "GLHelperMethods.h"
#include "GLBufferWrapper.h"

GLUnmapBufferCommand::GLUnmapBufferCommand(const ApiBufferWrapper* buffer):
	UnmapBufferCommand(buffer)
{

}

GLUnmapBufferCommand::~GLUnmapBufferCommand()
{

}

void GLUnmapBufferCommand::Execute()
{
	switch (m_buffer->As<GLBufferWrapper>()->GetType())
	{
	case BufferType::VERTEX:			GLCALL(glUnmapBuffer(GL_ARRAY_BUFFER));
	case BufferType::INDEX:				GLCALL(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
	case BufferType::COPY_SOURCE:		GLCALL(glUnmapBuffer(GL_COPY_READ_BUFFER));
	case BufferType::COPY_DESTINATION:	GLCALL(glUnmapBuffer(GL_COPY_WRITE_BUFFER));
	}
}