#pragma once

#include "graphics/Shader.h"
#include "Dx11ShaderUniform.h"

class Dx11CBuffer
{
public:
	// reg = register
	Dx11CBuffer(unsigned int reg, Shader::Type shaderType);
	~Dx11CBuffer();

	void PushUniform(const Dx11ShaderUniform& uniform);

private:
	std::vector<Dx11ShaderUniform> m_uniforms;
	unsigned int m_register;
	Shader::Type m_shader_type;
	unsigned int m_size;
	unsigned int m_real_size;
};