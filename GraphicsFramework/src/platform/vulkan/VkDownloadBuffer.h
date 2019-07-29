#pragma once

#include "graphics/DownloadBuffer.h"

class VkDownloadBuffer : public DownloadBuffer
{
public:
	VkDownloadBuffer();
	virtual ~VkDownloadBuffer();

	void Download(const VertexBuffer* vb);

private:
	VkBuffer m_buffer;
	VkDeviceMemory m_buffer_memory;
};