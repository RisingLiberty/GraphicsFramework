#pragma once

#include "graphics/IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(size_t count, BufferUsage usage, void* data);
	~OpenGLIndexBuffer();

	virtual void SetData(void* data) override;
	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
	unsigned int m_id;
};