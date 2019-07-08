#include "stdafx.h"

#include "VkVertexLayout.h"

VkVertexLayout::VkVertexLayout()
{
	m_binding_description.binding = 0;
	m_binding_description.stride = sizeof(float) * 3;
	m_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	m_attribute_descriptions.resize(1);

	m_attribute_descriptions[0].binding = 0;
	m_attribute_descriptions[0].location = 0;
	m_attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	m_attribute_descriptions[0].offset = 0;


	m_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_create_info.vertexBindingDescriptionCount = 1;
	m_create_info.pVertexBindingDescriptions = &m_binding_description;
	m_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attribute_descriptions.size());
	m_create_info.pVertexAttributeDescriptions = m_attribute_descriptions.data();
}

VkVertexLayout::~VkVertexLayout()
{

}

const VkPipelineVertexInputStateCreateInfo& VkVertexLayout::GetCreateInfo() const
{
	return m_create_info;
}