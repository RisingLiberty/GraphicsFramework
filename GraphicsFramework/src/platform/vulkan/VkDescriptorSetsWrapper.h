#pragma once

class VkDescriptorSetsWrapper
{
public:
	VkDescriptorSetsWrapper(unsigned int count, const std::vector<VkDescriptorSetLayout>& layouts, VkBuffer buffer, VkDescriptorPool descPool);
	~VkDescriptorSetsWrapper();

	std::vector<VkDescriptorSet> GetApiDescriptorSets() const;

private:
	std::vector<VkDescriptorSet> m_descriptor_sets;
};