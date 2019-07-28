#pragma once

#include "graphics/IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(unsigned int  count, Format format, Topology topology, BufferUsage usage, void* data);
	virtual ~OpenGLIndexBuffer();

	virtual void SetData(const void* data) override;

	unsigned int GetId() const;

private:
	unsigned int m_id;
};