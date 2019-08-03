#pragma once

class DownloadBuffer;

#include "graphics/ApiBufferWrapper.h"

class VkBufferWrapper : public ApiBufferWrapper
{
public:
	VkBufferWrapper(unsigned int size, BufferUsage usage, const void* data);
	virtual ~VkBufferWrapper();
	
	VkBuffer GetBufferGpu() const;

protected:
	void SetDataInternal(const void* data, unsigned int size) override;

	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer(unsigned int size) const;

protected:
	VkBuffer m_buffer_gpu;
	VkDeviceMemory m_buffer_memory_gpu;
	VkBuffer m_upload_buffer;
	VkDeviceMemory m_upload_buffer_memory;
};