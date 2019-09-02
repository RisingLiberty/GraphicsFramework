#pragma once

#include "graphics/IndexBuffer.h"
#include "VkBufferWrapper.h"

class VkIndexBuffer : public IndexBuffer, public VkBufferWrapper
{
public:
	VkIndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, BufferAccess access, const void* data);
	virtual ~VkIndexBuffer();

	std::unique_ptr<byte> GetData() const override;
	virtual void SetData(const void* datas) override;
};