#pragma once

#include "graphics/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(unsigned int size, BufferUsage usage, void* data);
	virtual ~OpenGLVertexBuffer();

	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer() const override;
	void SetData(const void* data) override;
	unsigned int GetId() const;

	virtual void Bind() const;
	virtual void Unbind() const;
private:
	unsigned int m_id;

};