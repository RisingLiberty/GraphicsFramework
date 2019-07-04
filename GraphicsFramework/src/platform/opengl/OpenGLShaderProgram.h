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
	void UploadVariables() override;

	unsigned int GetId() const { return m_id; }

	int GetUniformLocation(const std::string& name);

private:
	void Create(const std::vector<unsigned int>& shaders);

	void UploadBoolUniform(const std::string& name, bool uniform);
	void UploadUShortUniform(const std::string& name, unsigned short& uniform);
	void UploadShortUniform(const std::string& name, short uniform);
	void UploadIntUniform(const std::string& name, int uniform);
	void UploadUIntUniform(const std::string& name, unsigned int& uniform);
	void UploadFloatUniform(const std::string& name, float uniform);
	void UploadDoubleUniform(const std::string& name, double uniform);
	void UploadVec2Uniform(const std::string& name, float* values);
	void UploadVec3Uniform(const std::string& name, float* values);
	void UploadVec4Uniform(const std::string& name, float* values);
	void UploadMat3Uniform(const std::string& name, float* values);
	void UploadMat4Uniform(const std::string& name, float* values);


	void LoadUniforms();

private:
	unsigned int m_id;
	std::map<std::string, int> m_uniform_location_cache;
};
