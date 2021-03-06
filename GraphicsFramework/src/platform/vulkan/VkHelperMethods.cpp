#include "stdafx.h"

#include "VkHelperMethods.h"

#include "VkContext.h"

#include "VkCommandList.h"

VkContext* GetVkContext()
{
	return static_cast<VkContext*>(Context::GetCurrent());
}

VkDevice GetVkDevice()
{
	return GetVkContext()->GetDevice();
}

VkCommandList* GetVkCurrentCommandList()
{
	return GetVkContext()->GetCurrentCommandList();
}

VkCommandBuffer GetVkCurrentCommandBuffer()
{
	return GetVkContext()->GetCurrentCommandBuffer();
}

VkAllocationCallbacks* GetVkAllocationCallbacks()
{
	return VkContext::GetAllocationCallbacks();
}

uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties mem_properties;
	vkGetPhysicalDeviceMemoryProperties(GetVkContext()->GetSelectedGpu(), &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i)
	{
		if (typeFilter & (1 << i) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}

	ThrowException("failed to find suitable memory type!");
}

void CreateBuffer(size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = size;
	buffer_info.usage = usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VKCALL(vkCreateBuffer(GetVkDevice(), &buffer_info, GetVkAllocationCallbacks(), &buffer));

	VkMemoryRequirements mem_requirements;
	vkGetBufferMemoryRequirements(GetVkDevice(), buffer, &mem_requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, properties);

	VKCALL(vkAllocateMemory(GetVkDevice(), &alloc_info, GetVkAllocationCallbacks(), &bufferMemory));

	vkBindBufferMemory(GetVkDevice(), buffer, bufferMemory, 0);
}

void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	std::unique_ptr<VkCommandList> command_buffer = GetVkContext()->CreateDirectCommandList();

	VkBufferCopy copy_region = {};
	copy_region.srcOffset = 0;
	copy_region.dstOffset = 0;
	copy_region.size = size;

	vkCmdCopyBuffer(command_buffer->GetApiBuffer(), srcBuffer, dstBuffer, 1, &copy_region);
}