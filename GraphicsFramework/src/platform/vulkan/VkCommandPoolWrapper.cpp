#include "stdafx.h"

#include "VkCommandPoolWrapper.h"
#include "VkHelperMethods.h"

VkCommandPoolWrapper::VkCommandPoolWrapper(unsigned int familyIndex)
{
	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = familyIndex;
	pool_info.flags = 0;

	VKCALL(vkCreateCommandPool(GetVkDevice(), &pool_info, GetVkAllocationCallbacks(), &m_pool));
}

VkCommandPoolWrapper::~VkCommandPoolWrapper()
{

}

VkCommandPool VkCommandPoolWrapper::GetApiCommandPool() const
{
	return m_pool;
}