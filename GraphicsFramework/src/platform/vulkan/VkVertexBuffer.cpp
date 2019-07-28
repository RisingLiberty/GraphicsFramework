#include "stdafx.h"

#include "VkVertexBuffer.h"
#include "VkHelperMethods.h"

VkVertexBuffer::VkVertexBuffer(unsigned int size, BufferUsage usage, const void* data) :
	VertexBuffer(size, usage)
{
	if (data)
		this->SetData(data);
}

VkVertexBuffer::~VkVertexBuffer()
{
	vkDestroyBuffer(GetVkDevice(), m_buffer_gpu, nullptr);
	vkFreeMemory(GetVkDevice(), m_buffer_memory_gpu, nullptr);

}

void VkVertexBuffer::SetData(const void* vertices)
{
	CreateBuffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_upload_buffer, m_upload_buffer_memory);

	void* data;
	vkMapMemory(GetVkDevice(), m_upload_buffer_memory, 0, m_size, 0, &data);
	memcpy(data, vertices, (size_t)m_size);
	vkUnmapMemory(GetVkDevice(), m_upload_buffer_memory);

	CreateBuffer(m_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer_gpu, m_buffer_memory_gpu);
	CopyBuffer(m_upload_buffer, m_buffer_gpu, m_size);
	vkDestroyBuffer(GetVkDevice(), m_upload_buffer, nullptr);
	vkFreeMemory(GetVkDevice(), m_upload_buffer_memory, nullptr);
}

VkBuffer VkVertexBuffer::GetGpuBuffer() const
{
	return m_buffer_gpu;
}
