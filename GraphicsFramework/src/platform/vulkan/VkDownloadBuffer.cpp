#include "stdafx.h"

#include "VkDownloadBuffer.h"
#include "VkHelperMethods.h"
#include "VkVertexBuffer.h"

VkDownloadBuffer::VkDownloadBuffer()
{

}

VkDownloadBuffer::~VkDownloadBuffer()
{
	vkDestroyBuffer(GetVkDevice(), m_buffer, nullptr);
	vkFreeMemory(GetVkDevice(), m_buffer_memory, nullptr);
}

void VkDownloadBuffer::Download(const VertexBuffer* vb)
{
	const VkVertexBuffer* vk_vb = static_cast<const VkVertexBuffer*>(vb);

	CreateBuffer(vb->GetSize(), VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_buffer, m_buffer_memory);
	CopyBuffer(vk_vb->GetGpuBuffer(), m_buffer, vb->GetSize());

	m_data = malloc(vb->GetSize());
	void* data;
	vkMapMemory(GetVkDevice(), m_buffer_memory, 0, vb->GetSize(), 0, &data);
	memcpy(m_data, data, (size_t)vb->GetSize());
	vkUnmapMemory(GetVkDevice(), m_buffer_memory);


}