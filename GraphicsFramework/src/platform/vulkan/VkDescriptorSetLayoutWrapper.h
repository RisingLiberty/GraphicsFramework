#pragma once

class VkDescriptorSetLayoutWrapper
{
public:
	VkDescriptorSetLayoutWrapper();
	~VkDescriptorSetLayoutWrapper();

	VkDescriptorSetLayout GetApiDescriptorSetLayout() const;
private:
	VkDescriptorSetLayout m_descriptor_set_layout;
};