#pragma once

#include "graphics/VertexBuffer.h"
#include "GLBufferWrapper.h"

class GLVertexBuffer : public VertexBuffer, public GLBufferWrapper
{
public:
	GLVertexBuffer(unsigned int size, BufferUsage usage, BufferAccess access, const void* data);
	virtual ~GLVertexBuffer();

	void GLBind() const override;
	void SetData(const void* data) override;
	std::unique_ptr<byte> GetData() const override;
};