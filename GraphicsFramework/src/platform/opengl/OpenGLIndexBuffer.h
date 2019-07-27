#pragma once

#include "graphics/IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(size_t count, Format format, Topology topology, BufferUsage usage, void* data);
	virtual ~OpenGLIndexBuffer();

	virtual void SetData(const void* data) override;
	virtual void Bind() const override;
	virtual void Unbind() const override;

	unsigned int GetId() const;

private:
	unsigned int m_id;
};