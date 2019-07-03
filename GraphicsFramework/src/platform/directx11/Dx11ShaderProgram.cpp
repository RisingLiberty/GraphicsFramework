#include "stdafx.h"

#include "Dx11ShaderProgram.h"


Dx11ShaderProgram::Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
}


Dx11ShaderProgram::~Dx11ShaderProgram()
{
}

void Dx11ShaderProgram::Bind() const
{

}

void Dx11ShaderProgram::Unbind() const
{

}

void Dx11ShaderProgram::SetBoolUniform(const std::string & name, bool uniform)
{
}

void Dx11ShaderProgram::SetUShortUniform(const std::string & name, unsigned short & uniform)
{
}

void Dx11ShaderProgram::SetShortUniform(const std::string & name, short uniform)
{
}

void Dx11ShaderProgram::SetIntUniform(const std::string & name, int uniform)
{
}

void Dx11ShaderProgram::SetUIntUniform(const std::string & name, unsigned int & uniform)
{
}

void Dx11ShaderProgram::SetFloatUniform(const std::string & name, float uniform)
{
}

void Dx11ShaderProgram::SetDoubleUniform(const std::string & name, double uniform)
{
}

void Dx11ShaderProgram::SetVec2Uniform(const std::string & name, float x, float y)
{
}

void Dx11ShaderProgram::SetVec2Uniform(const std::string & name, float * values)
{
}

void Dx11ShaderProgram::SetVec3Uniform(const std::string & name, float x, float y, float z)
{
}

void Dx11ShaderProgram::SetVec3Uniform(const std::string & name, float * values)
{
}

void Dx11ShaderProgram::SetVec4Uniform(const std::string & name, float x, float y, float z, float w)
{
}

void Dx11ShaderProgram::SetVec4Uniform(const std::string & name, float * values)
{
}

void Dx11ShaderProgram::SetMat3Uniform(const std::string & name, float * values)
{
}

void Dx11ShaderProgram::SetMat4Uniform(const std::string & name, float * values)
{
}
