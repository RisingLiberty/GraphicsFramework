#pragma once

#include "graphics/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(size_t size, BufferUsage usage, void* data);
	virtual ~OpenGLVertexBuffer();

	virtual void SetData(const void* data) override;
	unsigned int GetId() const;

private:
	virtual void ForceBind() const;
	virtual void ForceUnbind() const;
	
private:
	unsigned int m_id;

};