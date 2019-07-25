#pragma once

#include "Shader.h"
#include "GraphicsResource.h"

class FragmentShader : public Shader, public GraphicsResource<FragmentShader>
{
public:
	FragmentShader(const std::string& path);
	~FragmentShader();

	// give the shader name without extention
	static FragmentShader* Create(const std::string& shaderName);

	Type GetType() const override final;

protected:
	virtual int Compile() override = 0;

};