#include "stdafx.h"

#include "VkShaderProgram.h"

#include "VkVertexShader.h"
#include "VkFragmentShader.h"

VkShaderProgram::VkShaderProgram(VertexShader* vs, FragmentShader* fs) :
	ShaderProgram(vs, fs)
{

}

VkShaderProgram::~VkShaderProgram()
{

}

std::array<VkPipelineShaderStageCreateInfo, 2> VkShaderProgram::GetPipelineShaderStageCreateInfos() const
{
	return
	{
		dynamic_cast<VkVertexShader*>(m_vertex_shader)->GetPipelineShaderStageCreateInfo(),
		dynamic_cast<VkFragmentShader*>(m_fragment_shader)->GetPipelineShaderStageCreateInfo()
	};
}

void VkShaderProgram::Bind() const
{
}

void VkShaderProgram::Unbind() const
{
}

void VkShaderProgram::UploadVariables()
{
}
