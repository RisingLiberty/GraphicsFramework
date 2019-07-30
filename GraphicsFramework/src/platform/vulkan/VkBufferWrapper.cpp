#include "stdafx.h"

#include "VkBufferWrapper.h"
#include "VkHelperMethods.h"
#include "VkDownloadBuffer.h"

VkBufferWrapper::VkBufferWrapper(unsigned int size, BufferUsage usage, const void* data):
	ApiBufferWrapper(usage)
{
	if (data)
		this->SetDataInternal(data, size);
}

VkBufferWrapper::~VkBufferWrapper()
{
	vkDestroyBuffer(GetVkDevice(), m_buffer_gpu, nullptr);
	vkFreeMemory(GetVkDevice(), m_buffer_memory_gpu, nullptr);
}

VkBuffer VkBufferWrapper::GetBufferGpu() const
{
	return m_buffer_gpu;
}

void VkBufferWrapper::SetDataInternal(const void* data, unsigned int size)
{
	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_upload_buffer, m_upload_buffer_memory);

	void* gpu_mem_ptr;
	vkMapMemory(GetVkDevice(), m_upload_buffer_memory, 0, size, 0, &gpu_mem_ptr);
	memcpy(gpu_mem_ptr, data, (size_t)size);
	vkUnmapMemory(GetVkDevice(), m_upload_buffer_memory);

	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer_gpu, m_buffer_memory_gpu);
	CopyBuffer(m_upload_buffer, m_buffer_gpu, size);
	vkDestroyBuffer(GetVkDevice(), m_upload_buffer, nullptr);
	vkFreeMemory(GetVkDevice(), m_upload_buffer_memory, nullptr);
}

std::unique_ptr<DownloadBuffer> VkBufferWrapper::DownloadDataToBuffer(unsigned int size) const
{
	std::unique_ptr<DownloadBuffer> buffer = std::make_unique<VkDownloadBuffer>(size);
	buffer->Download(this);
	return std::move(buffer);	
}