#include "stdafx.h"

#include "IndexBuffer.h"

#include "platform/opengl/OpenGLIndexBuffer.h"

#include "Context.h"

std::unique_ptr<IndexBuffer> IndexBuffer::Create(size_t count)
{
	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OpenGL:
		return std::make_unique<OpenGLIndexBuffer>(count);
	}

	return nullptr;
}

IndexBuffer::IndexBuffer(size_t count):
	m_count(count)
{

}

IndexBuffer::~IndexBuffer()
{

}

size_t IndexBuffer::GetCount() const
{
	return m_count;	
}