#pragma once

#include "graphics/VertexShader.h"

class VkVertexShader : public VertexShader
{
public:
	VkVertexShader(const std::string& path);
	virtual ~VkVertexShader();

	const std::string& GetCode() const;
	const VkPipelineShaderStageCreateInfo& GetPipelineShaderStageCreateInfo() const;

protected:
	virtual int Compile() override;

private:
	VkShaderModule m_module;
	VkPipelineShaderStageCreateInfo m_stage_info;
	std::string m_code;
};