#include "stdafx.h"

#include "VkVertexLayout.h"

VkVertexLayout::VkVertexLayout()
{
	m_binding_description.binding = 0;
	m_binding_description.stride = this->GetSize();
	m_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	m_attribute_descriptions.resize(m_attributes.size());

	unsigned int location = 0;
	unsigned int offset = 0;
	for (size_t i = 0; i < m_attribute_descriptions.size(); ++i)
	{
		m_attribute_descriptions[i].binding = 0;
		m_attribute_descriptions[i].location = location;
		m_attribute_descriptions[i].format = m_attributes[i].GetVulkanDataType();
		m_attribute_descriptions[i].offset = offset;

		offset += m_attributes[i].GetSize();
		++location;
	}

	m_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_create_info.vertexBindingDescriptionCount = 1;
	m_create_info.pVertexBindingDescriptions = &m_binding_description;
	m_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attribute_descriptions.size());
	m_create_info.pVertexAttributeDescriptions = m_attribute_descriptions.data();
}

VkVertexLayout::~VkVertexLayout()
{

}

const VkPipelineVertexInputStateCreateInfo& VkVertexLayout::GetCreateInfo()
{
	m_binding_description.binding = 0;
	m_binding_description.stride = this->GetSize();
	m_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	m_attribute_descriptions.resize(m_attributes.size());

	unsigned int location = 0;
	unsigned int offset = 0;
	for (size_t i = 0; i < m_attribute_descriptions.size(); ++i)
	{
		m_attribute_descriptions[i].binding = 0;
		m_attribute_descriptions[i].location = location;
		m_attribute_descriptions[i].format = m_attributes[i].GetVulkanDataType();
		m_attribute_descriptions[i].offset = offset;

		offset += m_attributes[i].GetSize();
		++location;
	}

	m_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_create_info.vertexBindingDescriptionCount = 1;
	m_create_info.pVertexBindingDescriptions = &m_binding_description;
	m_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attribute_descriptions.size());
	m_create_info.pVertexAttributeDescriptions = m_attribute_descriptions.data();

	return m_create_info;
}