#pragma once

#include "graphics/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(size_t size, BufferUsage usage, void* data = nullptr);
	virtual ~OpenGLVertexBuffer();

	
	virtual void SetData(void* data) override;
	virtual void Bind() const override;
	virtual void Unbind() const override;
	
private:
	unsigned int m_id;

};