#include "stdafx.h"

#include "VkDownloadBuffer.h"
#include "VkHelperMethods.h"
#include "VkVertexBuffer.h"

VkDownloadBuffer::VkDownloadBuffer(unsigned int size):
	DownloadBuffer(size)
{

}

VkDownloadBuffer::~VkDownloadBuffer()
{
	vkDestroyBuffer(GetVkDevice(), m_buffer, GetVkAllocationCallbacks());
	vkFreeMemory(GetVkDevice(), m_buffer_memory, GetVkAllocationCallbacks());
}

void VkDownloadBuffer::Download(const ApiBufferWrapper* buffer)
{
	const VkVertexBuffer* vk_vb = static_cast<const VkVertexBuffer*>(buffer);

	CreateBuffer(m_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_buffer, m_buffer_memory);
	CopyBuffer(vk_vb->GetBufferGpu(), m_buffer, m_size);

	void* data;
	vkMapMemory(GetVkDevice(), m_buffer_memory, 0, m_size, 0, &data);
	memcpy(m_cpu_address, data, (size_t)m_size);
	vkUnmapMemory(GetVkDevice(), m_buffer_memory);
}