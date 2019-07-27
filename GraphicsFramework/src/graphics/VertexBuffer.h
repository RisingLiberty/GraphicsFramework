#pragma once

#include "BufferHelpers.h"
#include "GraphicsResource.h"

class VertexBuffer : public GraphicsResource<VertexBuffer>
{
public:

	VertexBuffer(size_t size, BufferUsage usage);
	virtual ~VertexBuffer();

	virtual void SetData(const void* data) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	BufferUsage GetUsage() const;
	virtual size_t GetSize() const;

	static VertexBuffer* Create(size_t size, BufferUsage usage = BufferUsage::STATIC, void* data = nullptr);

	bool operator==(const VertexBuffer* other) const;

protected:
	size_t m_size;
	BufferUsage m_usage;

};