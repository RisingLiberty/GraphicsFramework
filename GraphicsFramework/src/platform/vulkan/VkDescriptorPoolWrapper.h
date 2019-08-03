#pragma once

class VkDescriptorPoolWrapper
{
public:
	VkDescriptorPoolWrapper(unsigned int maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes);
	~VkDescriptorPoolWrapper();

	VkDescriptorPool GetDescriptorPool() const;

private:
	VkDescriptorPool m_descriptor_pool;
};