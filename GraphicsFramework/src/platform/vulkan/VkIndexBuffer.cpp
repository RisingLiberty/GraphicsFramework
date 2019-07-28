#include "stdafx.h"

#include "VkIndexBuffer.h"
#include "VkHelperMethods.h"

#include "VkContext.h"

VkIndexBuffer::VkIndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, const void* data):
	IndexBuffer(count, format, topology, usage)
{
	if (data)
		this->SetData(data);
}

VkIndexBuffer::~VkIndexBuffer()
{
	vkDestroyBuffer(GetVkDevice(), m_buffer_gpu, nullptr);
	vkFreeMemory(GetVkDevice(), m_buffer_memory_gpu, nullptr);
}

void VkIndexBuffer::SetData(const void* indices)
{
	size_t size = this->GetSize();

	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_upload_buffer, m_upload_buffer_memory);

	void* data;
	vkMapMemory(GetVkDevice(), m_upload_buffer_memory, 0, size, 0, &data);
	memcpy(data, indices, size);
	vkUnmapMemory(GetVkDevice(), m_upload_buffer_memory);

	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer_gpu, m_buffer_memory_gpu);
	CopyBuffer(m_upload_buffer, m_buffer_gpu, size);
	vkDestroyBuffer(GetVkDevice(), m_upload_buffer, nullptr);
	vkFreeMemory(GetVkDevice(), m_upload_buffer_memory, nullptr);
}

VkBuffer VkIndexBuffer::GetGpuBuffer() const
{
	return m_buffer_gpu;
}