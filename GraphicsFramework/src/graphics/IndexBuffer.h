#pragma once

#include "GraphicsResource.h"
#include "BufferHelpers.h"

class IndexBuffer : public GraphicsResource<IndexBuffer>
{
public:
	IndexBuffer(size_t count, BufferUsage usage, Format format = EFormat::R32_UINT, Topology topology = ETopology::TRIANGLELIST);
	virtual ~IndexBuffer();

	virtual void SetData(const void* data) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	size_t GetCount() const;
	size_t GetSize() const;
	Format GetFormat() const;
	Topology GetTopology() const;

	static IndexBuffer* Create(size_t count, Format format, Topology topology, BufferUsage usage = BufferUsage::STATIC, void* data = nullptr);

protected:
	size_t m_count;
	BufferUsage m_usage;
	Format m_format;
	Topology m_topology;
};