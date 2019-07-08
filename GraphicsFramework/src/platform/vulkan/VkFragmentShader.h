#pragma once

#include "graphics/FragmentShader.h"

class VkFragmentShader : public FragmentShader
{
public:
	VkFragmentShader(const std::string& path);
	virtual ~VkFragmentShader();

	const std::string& GetCode() const;
	const VkPipelineShaderStageCreateInfo& GetPipelineShaderStageCreateInfo() const;


protected:
	int Compile();

private:
	VkShaderModule m_module;
	VkPipelineShaderStageCreateInfo m_stage_info;
	std::string m_code;

};