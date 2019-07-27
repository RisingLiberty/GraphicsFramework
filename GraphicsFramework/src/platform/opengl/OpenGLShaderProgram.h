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

	void UploadVariables() const override;

	unsigned int GetId() const { return m_id; }

	int GetUniformLocation(const std::string& name) const;

private:
	void Create(const std::vector<unsigned int>& shaders);

	void UploadBoolUniform(const std::string& name, bool uniform) const;
	void UploadUShortUniform(const std::string& name, unsigned short& uniform) const;
	void UploadShortUniform(const std::string& name, short uniform) const;
	void UploadIntUniform(const std::string& name, int uniform) const;
	void UploadUIntUniform(const std::string& name, unsigned int& uniform) const;
	void UploadFloatUniform(const std::string& name, float uniform) const;
	void UploadDoubleUniform(const std::string& name, double uniform) const;
	void UploadVec2Uniform(const std::string& name, float* values) const;
	void UploadVec3Uniform(const std::string& name, float* values) const;
	void UploadVec4Uniform(const std::string& name, float* values) const;
	void UploadMat3Uniform(const std::string& name, float* values) const;
	void UploadMat4Uniform(const std::string& name, float* values) const;


	void LoadUniforms();

private:
	unsigned int m_id;
	mutable std::map<std::string, int> m_uniform_location_cache;
};
