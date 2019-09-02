#include "stdafx.h"

#include "GLDownloadBuffer.h"
#include "GLVertexBuffer.h"
#include "GLHelperMethods.h"

#include "GLCreateBufferCommand.h"
#include "GLBindCopyDestBufferCommand.h"
#include "GLSetBufferDataCommand.h"

#include "GLBindCopySrcBufferCommand.h"
#include "GLBindCopyDestBufferCommand.h"
#include "GLCopyBufferCommand.h"

#include "GLDeleteBufferCommand.h"

#include "GLMapBufferCommand.h"
#include "graphics/CopyToPointerCommand.h"
#include "GLUnmapBufferCommand.h"

#include "GLDirectCommandList.h"

namespace
{
	unsigned int NULL_OFFSET = 0;
}

GLDownloadBuffer::GLDownloadBuffer(unsigned int size):
	GLBufferWrapper(size, EBufferUsage::STATIC, EBufferAccess::READ, BufferType::COPY_DESTINATION, nullptr),
	DownloadBuffer(size)
{
	m_cmd_list = GetGLContext()->CreateDirectCommandList();
	this->GLBind();

	m_cmd_list->Push(std::make_unique<GLSetBufferDataCommand>(this, m_size, nullptr, BufferType::COPY_DESTINATION));
	//GLCALL(glGenBuffers(1, &m_buffer_id));
	//GLCALL(glBindBuffer(GL_COPY_WRITE_BUFFER, m_buffer_id));
	//GLCALL(glBufferStorage(GL_COPY_WRITE_BUFFER, m_size, nullptr, GL_MAP_READ_BIT));
}

GLDownloadBuffer::~GLDownloadBuffer()
{
	//GLCALL(glDeleteBuffers(1, &m_buffer_id));
}

void GLDownloadBuffer::Download(const ApiBufferWrapper* buffer)
{
	m_cmd_list->Push(std::make_unique<GLBindCopySrcBufferCommand>(buffer));
	this->GLBind();
	m_cmd_list->Push(std::make_unique<GLCopyBufferCommand>(m_size, this, buffer));
	//GLCALL(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, NULL_OFFSET, NULL_OFFSET, m_size));

	//void* copied_data = glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, m_size, GL_MAP_READ_BIT);
	//memcpy(m_cpu_address, copied_data, m_size);
	//GLCALL(glUnmapBuffer(GL_COPY_WRITE_BUFFER));

	m_cmd_list->Push(std::make_unique<GLMapBufferCommand>(this, &m_gpu_address, m_size));
	m_cmd_list->Push(std::make_unique<CopyToPointerCommand>(m_cpu_address, &m_gpu_address, m_size));
	m_cmd_list->Push(std::make_unique<GLUnmapBufferCommand>(this));

	m_cmd_list->Close();
	m_cmd_list->Execute();
}

void GLDownloadBuffer::GLBind() const
{
	m_cmd_list->Push(std::make_unique<GLBindCopyDestBufferCommand>(this));
}