#include "stdafx.h"

#include "VkVertexShader.h"
#include "VkHelperMethods.h"

namespace
{
	static std::vector<char> ReadFile(const std::string& path)
	{
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			ThrowException("Failed to open vulkan shader file!");

		size_t file_size = (size_t)file.tellg();
		std::vector<char> buffer(file_size);

		file.seekg(0);
		file.read(buffer.data(), file_size);

		file.close();

		return buffer;
	}

}

VkVertexShader::VkVertexShader(const std::string& path) :
	VertexShader(path)
{
	std::vector<char> code = ReadFile(path);
	m_code = std::string(code.begin(), code.end());

	VkShaderModuleCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = m_code.size();
	create_info.pCode = reinterpret_cast<uint32_t*>(m_code.data());

	VKCALL(vkCreateShaderModule(GetVkDevice(), &create_info, GetVkAllocationCallbacks(), &m_module));
	m_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	m_stage_info.module = m_module;
	m_stage_info.pName = "main";
}

VkVertexShader::~VkVertexShader()
{
	vkDestroyShaderModule(GetVkDevice(), m_module, GetVkAllocationCallbacks());
}

int VkVertexShader::Compile()
{
	return 0;
}

const std::string& VkVertexShader::GetCode() const
{
	return m_code;
}

const VkPipelineShaderStageCreateInfo& VkVertexShader::GetPipelineShaderStageCreateInfo() const
{
	return m_stage_info;
}