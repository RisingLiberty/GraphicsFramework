#include "stdafx.h"

#include "VkShaderProgram.h"
#include "VkHelperMethods.h"
#include "VkContext.h"

#include "VkVertexShader.h"
#include "VkFragmentShader.h"

#include "VkBufferWrapper.h"

VkShaderProgram::VkShaderProgram(VertexShader* vs, FragmentShader* fs) :
	ShaderProgram(vs, fs)
{
	GetVkContext()->BindShaderProgram(this);

	unsigned int size = sizeof(float) * 4;
	CreateBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniform_buffer, m_uniform_buffer_memory);

	float color[] = { 1.0f, 0.3f, 0.8f, 1.0f };
	m_uniforms.push_back(std::make_unique<ShaderUniform>("u_Color", UniformDataType::VEC4, size, (char*)color));
}

VkShaderProgram::~VkShaderProgram()
{

}

std::array<VkPipelineShaderStageCreateInfo, 2> VkShaderProgram::GetPipelineShaderStageCreateInfos() const
{
	return
	{
		static_cast<VkVertexShader*>(m_vertex_shader)->GetPipelineShaderStageCreateInfo(),
		static_cast<VkFragmentShader*>(m_fragment_shader)->GetPipelineShaderStageCreateInfo()
	};
}

void VkShaderProgram::UploadVariables() const
{
	unsigned int offset = 0;
	for (const std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
	{
		byte* data;
		vkMapMemory(GetVkDevice(), m_uniform_buffer_memory, 0, uniform->size, 0, (void**)&data);
		memcpy(&data[offset], uniform->data, uniform->size);
		vkUnmapMemory(GetVkDevice(), m_uniform_buffer_memory);
		offset += uniform->size;
	}
}

VkBuffer VkShaderProgram::GetUniformBuffer() const
{
	return m_uniform_buffer;
}

VkDeviceMemory VkShaderProgram::GetUniformBufferMemory() const
{
	return m_uniform_buffer_memory;
}