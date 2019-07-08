#pragma once

#include "graphics/VertexLayout.h"

class VkVertexLayout : public VertexLayout
{
public:
	VkVertexLayout();
	virtual ~VkVertexLayout();

	const VkPipelineVertexInputStateCreateInfo& GetCreateInfo();

private:
	VkPipelineVertexInputStateCreateInfo m_create_info;
	std::vector<VkVertexInputAttributeDescription> m_attribute_descriptions;
	VkVertexInputBindingDescription m_binding_description;
};