#pragma once

#include "graphics/ShaderProgram.h"

class VkShaderProgram : public ShaderProgram
{
public:
	VkShaderProgram(VertexShader* vs, FragmentShader* fs);
	~VkShaderProgram();

	std::array<VkPipelineShaderStageCreateInfo, 2> GetPipelineShaderStageCreateInfos() const;

	VkBuffer GetUniformBuffer() const;
	VkDeviceMemory GetUniformBufferMemory() const;

	virtual void UploadVariables() const override;

private:
	VkBuffer m_uniform_buffer;
	VkDeviceMemory m_uniform_buffer_memory;
};