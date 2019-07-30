#pragma once

#include "graphics/IndexBuffer.h"
#include "OpenGLBufferWrapper.h"

class OpenGLIndexBuffer : public IndexBuffer, public OpenGLBufferWrapper
{
public:
	OpenGLIndexBuffer(unsigned int  count, Format format, Topology topology, BufferUsage usage, void* data);
	virtual ~OpenGLIndexBuffer();

	void GLBind() const override;

	std::unique_ptr<byte> GetData() const;
	void SetData(const void* data) override;

	unsigned int GetId() const;

private:
	unsigned int m_id;
};