#pragma once

#include "graphics/VertexBuffer.h"
#include "VkBufferWrapper.h"

class VkVertexBuffer : public VertexBuffer, public VkBufferWrapper
{
public:
	VkVertexBuffer(unsigned int size, BufferUsage usage, const void* data);
	virtual ~VkVertexBuffer();

	std::unique_ptr<byte> GetData() const override;
	void SetData(const void* data) override;
};