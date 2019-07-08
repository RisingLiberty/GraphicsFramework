#pragma once

#include "graphics/VertexBuffer.h"

class VkVertexBuffer : public VertexBuffer
{
public:
	VkVertexBuffer(size_t size, BufferUsage usage, const void* data);
	~VkVertexBuffer();

	void SetData(const void* vertices) override;
	VkBuffer GetGpuBuffer() const;

	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
	VkBuffer m_buffer_gpu;
	VkDeviceMemory m_buffer_memory_gpu;
	VkBuffer m_upload_buffer;
	VkDeviceMemory m_upload_buffer_memory;
};