#pragma once

#include "Shader.h"

class FragmentShader : public Shader
{
public:
	FragmentShader(const std::string& path);
	~FragmentShader();

	virtual void Bind() const override = 0;
	virtual void Unbind() const override = 0;

	static FragmentShader* Create(const std::string& path);

protected:
	virtual int Compile() override = 0;
};