#pragma once

#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(const std::string& path);
	virtual ~VertexShader();

	virtual void Bind() const override = 0;
	virtual void Unbind() const override = 0;
	virtual int Compile() override = 0;


	static std::unique_ptr<VertexShader> Create(const std::string& path);
};