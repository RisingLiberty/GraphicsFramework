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

	virtual void SetBoolUniform(const std::string& name, bool uniform) override;
	virtual void SetUShortUniform(const std::string& name, unsigned short& uniform) override;
	virtual void SetShortUniform(const std::string& name, short uniform) override;
	virtual void SetIntUniform(const std::string& name, int uniform) override;
	virtual void SetUIntUniform(const std::string& name, unsigned int& uniform) override;
	virtual void SetFloatUniform(const std::string& name, float uniform) override;
	virtual void SetDoubleUniform(const std::string& name, double uniform) override;
	virtual void SetVec2Uniform(const std::string& name, float x, float y) override;
	virtual void SetVec2Uniform(const std::string& name, float* values) override;
	virtual void SetVec3Uniform(const std::string& name, float x, float y, float z) override;
	virtual void SetVec3Uniform(const std::string& name, float* values) override;
	virtual void SetVec4Uniform(const std::string& name, float x, float y, float z, float w) override;
	virtual void SetVec4Uniform(const std::string& name, float* values) override;
	virtual void SetMat3Uniform(const std::string& name, float* values) override;
	virtual void SetMat4Uniform(const std::string& name, float* values) override;

	int GetUniformLocation(const std::string& name);

private:
	void Create(const std::vector<unsigned int>& shaders);

	void LoadUniforms();

private:
	unsigned int m_id;
	std::map<std::string, int> m_uniform_location_cache;
};
