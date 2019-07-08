#pragma once

#define VKCALL(x)\
{\
	if(x != VK_SUCCESS)\
		ThrowException("Vulkan function call failed!");\
}

class VkContext* GetVkContext();
VkDevice GetVkDevice();
VkInstance GetVkInstance();
uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
void CreateBuffer(size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);