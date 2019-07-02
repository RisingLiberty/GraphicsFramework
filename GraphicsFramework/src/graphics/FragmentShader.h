#pragma once

#include "Shader.h"

class FragmentShader : public Shader
{
public:
	FragmentShader(const std::string& path);
	~FragmentShader();

	// give the shader name without extention
	static FragmentShader* Create(const std::string& shaderName);

protected:
	virtual int Compile() override = 0;
};