#pragma once

#include "Shader.h"

class FragmentShader : public Shader
{
public:
	FragmentShader(const std::string& path);
	~FragmentShader();

	static FragmentShader* Create(const std::string& path);

protected:
	virtual int Compile() override = 0;
};