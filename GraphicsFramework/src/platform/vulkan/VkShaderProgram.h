#pragma once

#include "graphics/ShaderProgram.h"

class VkShaderProgram : public ShaderProgram
{
public:
	VkShaderProgram(VertexShader* vs, FragmentShader* fs);
	~VkShaderProgram();

	std::array<VkPipelineShaderStageCreateInfo, 2> GetPipelineShaderStageCreateInfos() const;

	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual void UploadVariables() override;

private:

};