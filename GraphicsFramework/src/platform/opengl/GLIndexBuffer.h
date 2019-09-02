#pragma once

#include "graphics/IndexBuffer.h"
#include "GLBufferWrapper.h"

class GLIndexBuffer : public IndexBuffer, public GLBufferWrapper
{
public:
	GLIndexBuffer(unsigned int  count, Format format, Topology topology, BufferUsage usage, BufferAccess access, void* data);
	virtual ~GLIndexBuffer();

	void GLBind() const override;

	virtual std::unique_ptr<byte> GetData() const override;
	virtual void SetData(const void* data) override;

};