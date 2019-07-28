#pragma once

#include "graphics/IndexBuffer.h"

class VkIndexBuffer : public IndexBuffer
{
public:
	VkIndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, const void* data);
	virtual ~VkIndexBuffer();

	virtual void SetData(const void* indices) override;

	VkBuffer GetGpuBuffer() const;

private:
	VkBuffer m_buffer_gpu;
	VkDeviceMemory m_buffer_memory_gpu;
	VkBuffer m_upload_buffer;
	VkDeviceMemory m_upload_buffer_memory;
};