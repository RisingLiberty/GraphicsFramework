#pragma once

#include "BufferHelpers.h"
#include "GraphicsResource.h"

class VertexBuffer : public GraphicsResource<VertexBuffer>
{
public:

	VertexBuffer(unsigned int size, BufferUsage usage);
	virtual ~VertexBuffer();

	virtual void SetData(const void* data) = 0;

	BufferUsage GetUsage() const;
	virtual size_t GetSize() const;

	static VertexBuffer* Create(unsigned int size, BufferUsage usage = BufferUsage::STATIC, void* data = nullptr);

	bool operator==(const VertexBuffer* other) const;

protected:
	unsigned int m_size;
	BufferUsage m_usage;

};