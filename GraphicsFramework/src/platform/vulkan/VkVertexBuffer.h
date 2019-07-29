#pragma once

#include "graphics/VertexBuffer.h"

class VkVertexBuffer : public VertexBuffer
{
public:
	VkVertexBuffer(unsigned int size, BufferUsage usage, const void* data);
	virtual ~VkVertexBuffer();

	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer() const;
	void SetData(const void* vertices) override;

	VkBuffer GetGpuBuffer() const;

private:
	VkBuffer m_buffer_gpu;
	VkDeviceMemory m_buffer_memory_gpu;
	VkBuffer m_upload_buffer;
	VkDeviceMemory m_upload_buffer_memory;
};