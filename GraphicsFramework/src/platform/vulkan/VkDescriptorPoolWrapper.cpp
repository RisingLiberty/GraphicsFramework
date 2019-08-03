#include "stdafx.h"

#include "VkDescriptorPoolWrapper.h"
#include "VkHelperMethods.h"

VkDescriptorPoolWrapper::VkDescriptorPoolWrapper(unsigned int maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes)
{
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	pool_info.pPoolSizes = poolSizes.data();
	pool_info.maxSets = static_cast<uint32_t>(maxSets);

	VKCALL(vkCreateDescriptorPool(GetVkDevice(), &pool_info, nullptr, &m_descriptor_pool));
}

VkDescriptorPoolWrapper::~VkDescriptorPoolWrapper()
{
	vkDestroyDescriptorPool(GetVkDevice(), m_descriptor_pool, nullptr);
}

VkDescriptorPool VkDescriptorPoolWrapper::GetDescriptorPool() const
{
	return m_descriptor_pool;
}