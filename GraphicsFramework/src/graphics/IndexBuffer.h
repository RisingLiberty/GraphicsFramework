#pragma once

#include "GraphicsResource.h"
#include "BufferHelpers.h"

class IndexBuffer : public GraphicsResource<IndexBuffer>
{
public:
	IndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage);
	virtual ~IndexBuffer();

	virtual void SetData(const void* data) = 0;
	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;
	unsigned int GetSize() const;
	Format GetFormat() const;
	Topology GetTopology() const;

	static IndexBuffer* Create(unsigned int count, Format format, Topology topology, BufferUsage usage = BufferUsage::STATIC, void* data = nullptr);

protected:
	unsigned int m_count;
	Format m_format;
	Topology m_topology;
	BufferUsage m_usage;
};