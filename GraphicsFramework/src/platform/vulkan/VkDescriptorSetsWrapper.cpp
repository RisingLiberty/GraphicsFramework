#include "stdafx.h"

#include "VkDescriptorSetsWrapper.h"
#include "VkHelperMethods.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <glm/gtc/matrix_transform.hpp>

struct UniformBufferObject
{
	glm::vec4 color;
};

VkDescriptorSetsWrapper::VkDescriptorSetsWrapper(unsigned int count, const std::vector<VkDescriptorSetLayout>& layouts, VkBuffer buffer, VkDescriptorPool descPool)
{
	VkDescriptorSetAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = descPool;
	alloc_info.descriptorSetCount = count;
	alloc_info.pSetLayouts = layouts.data();

	m_descriptor_sets.resize(count);
	vkAllocateDescriptorSets(GetVkDevice(), &alloc_info, m_descriptor_sets.data());

	for (size_t i = 0; i < count; ++i)
	{
		VkDescriptorBufferInfo buffer_info = {};
		buffer_info.buffer = buffer;
		buffer_info.offset = 0;
		buffer_info.range = sizeof(UniformBufferObject);

		//VkDescriptorImageInfo image_info = {};
		//image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//image_info.imageView = m_texture_image_view;
		//image_info.sampler = m_texture_sampler;

		std::array<VkWriteDescriptorSet, 1> descriptor_writes = {};
		descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_writes[0].dstSet = m_descriptor_sets[i];
		descriptor_writes[0].dstBinding = 0;
		descriptor_writes[0].dstArrayElement = 0;
		descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor_writes[0].descriptorCount = 1;
		descriptor_writes[0].pBufferInfo = &buffer_info;
		descriptor_writes[0].pImageInfo = nullptr;
		descriptor_writes[0].pTexelBufferView = nullptr;

		//descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//descriptor_writes[1].dstSet = m_descriptor_sets[i];
		//descriptor_writes[1].dstBinding = 1;
		//descriptor_writes[1].dstArrayElement = 0;
		//descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//descriptor_writes[1].descriptorCount = 1;

		//descriptor_writes[1].pImageInfo = &image_info;

		vkUpdateDescriptorSets(GetVkDevice(), static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
	}
}

VkDescriptorSetsWrapper::~VkDescriptorSetsWrapper()
{

}

std::vector<VkDescriptorSet> VkDescriptorSetsWrapper::GetApiDescriptorSets() const
{
	return m_descriptor_sets;
}