#pragma once

#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(const std::string& path);
	virtual ~VertexShader();

	static VertexShader* Create(const std::string& path);
protected:
	virtual int Compile() override = 0;
};