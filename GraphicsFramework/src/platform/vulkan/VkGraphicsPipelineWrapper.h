#pragma once

#include "graphics/BufferHelpers.h"
#include "graphics/RasterizerSettings.h"

class VkGraphicsPipelineWrapper
{
public:
	VkGraphicsPipelineWrapper(unsigned int width, unsigned int height,
		VkExtent2D extent, VkSampleCountFlagBits maxSampleCount,
		Topology topology, VkDescriptorSetLayout layout,
		RasterizerSettings rasterizerSettings, VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo,
		VkPipelineShaderStageCreateInfo* shaderStageCreateInfo, VkRenderPass renderPass);
	~VkGraphicsPipelineWrapper();

	VkPipelineLayout GetLayout() const;
	VkPipeline GetPipeline() const;

private:
	VkPipelineLayout m_layout;
	VkPipeline m_pipeline;
};