#pragma once

#include "OpenGLHelperMethods.h"

#include <graphics/ShaderUniform.h>
#include <graphics/ShaderProgram.h>

#include <GL/glew.h>

class OpenGLShaderProgram : public ShaderProgram
{
public:
	OpenGLShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~OpenGLShaderProgram();

	void Bind() const override;
	void Unbind() const override;

	unsigned int GetId() const { return m_id; }

	virtual void SetBoolUniform(const std::string& name, bool uniform);
	virtual void SetUShortUniform(const std::string& name, unsigned short& uniform);
	virtual void SetShortUniform(const std::string& name, short uniform);
	virtual void SetIntUniform(const std::string& name, int uniform);
	virtual void SetUIntUniform(const std::string& name, unsigned int& uniform);
	virtual void SetFloatUniform(const std::string& name, float uniform);
	virtual void SetDoubleUniform(const std::string& name, double uniform);
	virtual void SetVec2Uniform(const std::string& name, float x, float y);
	virtual void SetVec2Uniform(const std::string& name, float* values);
	virtual void SetVec3Uniform(const std::string& name, float x, float y, float z);
	virtual void SetVec3Uniform(const std::string& name, float* values);
	virtual void SetVec4Uniform(const std::string& name, float x, float y, float z, float w);
	virtual void SetVec4Uniform(const std::string& name, float* values);
	virtual void SetMat3Uniform(const std::string& name, float* values);
	virtual void SetMat4Uniform(const std::string& name, float* values);

	int GetUniformLocation(const std::string& name);

	std::vector<ShaderUniform> GetUniforms() const override;

private:
	void Create(const std::vector<unsigned int>& shaders);

private:
	unsigned int m_id;
	std::map<std::string, int> m_UniformLocationCache;
};
