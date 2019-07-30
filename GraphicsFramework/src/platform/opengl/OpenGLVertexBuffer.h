#pragma once

#include "graphics/VertexBuffer.h"
#include "OpenGLBufferWrapper.h"

class OpenGLVertexBuffer : public VertexBuffer, public OpenGLBufferWrapper
{
public:
	OpenGLVertexBuffer(unsigned int size, BufferUsage usage, const void* data);
	virtual ~OpenGLVertexBuffer();

	void GLBind() const override;
	void SetData(const void* data) override;
	std::unique_ptr<byte> GetData() const override;
};