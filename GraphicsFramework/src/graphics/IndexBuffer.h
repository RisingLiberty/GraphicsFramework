#pragma once

#include "BufferHelpers.h"
#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(unsigned int count, Format format, Topology topology);
	virtual ~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;
	Format GetFormat() const;
	Topology GetTopology() const;

	static IndexBuffer* Create(unsigned int count, Format format, Topology topology, BufferUsage usage = EBufferUsage::STATIC, BufferAccess access = EBufferAccess::NO_ACCESS, void* data = nullptr);

protected:
	unsigned int m_count;
	Format m_format;
	Topology m_topology;
};