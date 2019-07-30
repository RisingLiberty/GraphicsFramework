#pragma once

#include "graphics/DownloadBuffer.h"

class VkDownloadBuffer : public DownloadBuffer
{
public:
	VkDownloadBuffer(unsigned int size);
	virtual ~VkDownloadBuffer();

	void Download(const ApiBufferWrapper* vb);

private:
	VkBuffer m_buffer;
	VkDeviceMemory m_buffer_memory;
};