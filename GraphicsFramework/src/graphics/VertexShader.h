#pragma once

#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(const std::string& path);
	virtual ~VertexShader();

	// give the shader name without extention
	static VertexShader* Create(const std::string& shaderName);

	Type GetType() const override final;
protected:
	virtual int Compile() override = 0;
};