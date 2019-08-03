#include "stdafx.h"

#include "VkDescriptorSetLayoutWrapper.h"
#include "VkHelperMethods.h"

VkDescriptorSetLayoutWrapper::VkDescriptorSetLayoutWrapper()
{
	VkDescriptorSetLayoutBinding ubo_layout_binding = {};
	ubo_layout_binding.binding = 0;
	ubo_layout_binding.descriptorCount = 1;
	ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ubo_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ubo_layout_binding.pImmutableSamplers = nullptr;

	//VkDescriptorSetLayoutBinding sampler_layout_binding = {};
	//sampler_layout_binding.binding = 1;
	//sampler_layout_binding.descriptorCount = 1;
	//sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//sampler_layout_binding.pImmutableSamplers = nullptr;

	//sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { ubo_layout_binding };

	VkDescriptorSetLayoutCreateInfo layout_info = {};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	VKCALL(vkCreateDescriptorSetLayout(GetVkDevice(), &layout_info, nullptr, &m_descriptor_set_layout));
}

VkDescriptorSetLayoutWrapper::~VkDescriptorSetLayoutWrapper()
{
	vkDestroyDescriptorSetLayout(GetVkDevice(), m_descriptor_set_layout, nullptr);
}

VkDescriptorSetLayout VkDescriptorSetLayoutWrapper::GetApiDescriptorSetLayout() const
{
	return m_descriptor_set_layout;
}
