#pragma once

#include "BufferUsage.h"

class VertexBuffer
{
public:

	VertexBuffer(size_t size, BufferUsage usage);
	virtual ~VertexBuffer();

	virtual void SetData(void* data) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual size_t GetSize() const;

	static std::unique_ptr<VertexBuffer> Create(size_t size, BufferUsage usage = BufferUsage::STATIC, void* data = nullptr);

protected:
	size_t m_size;
	BufferUsage m_usage;

};