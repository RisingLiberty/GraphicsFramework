#pragma once

#define VKCALL(x)\
{\
	if(x != VK_SUCCESS)\
		ThrowException("Vulkan function call failed!");\
}

class VkContext* GetVkContext();