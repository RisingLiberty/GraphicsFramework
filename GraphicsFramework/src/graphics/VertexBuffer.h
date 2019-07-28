#pragma once

#include "BufferHelpers.h"
#include "GraphicsResource.h"

class DownloadBuffer;

class VertexBuffer : public GraphicsResource<VertexBuffer>
{
public:

	VertexBuffer(unsigned int size, BufferUsage usage);
	virtual ~VertexBuffer();

	virtual std::unique_ptr<DownloadBuffer> DownloadDataToBuffer() const = 0;
	virtual void SetData(const void* data) = 0;

	BufferUsage GetUsage() const;
	virtual size_t GetSize() const;

	static VertexBuffer* Create(unsigned int size, BufferUsage usage = BufferUsage::STATIC, void* data = nullptr);

	bool operator==(const VertexBuffer* other) const;

protected:
	unsigned int m_size;
	BufferUsage m_usage;

};