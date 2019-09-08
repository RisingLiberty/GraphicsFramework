#pragma once

#include "GLHelperMethods.h"

#include <graphics/ShaderUniform.h>
#include <graphics/ShaderProgram.h>

#include <GL/glew.h>

class GLShaderProgram : public ShaderProgram
{
public:
	GLShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~GLShaderProgram();

	void UploadVariables() const override;

	unsigned int GetId() const { return m_id; }

	int GetUniformLocation(const std::string& name) const;

private:
	void Create(const std::vector<unsigned int>& shaders);

	void LoadUniforms();

private:
	unsigned int m_id;
	mutable std::map<std::string, int> m_uniform_location_cache;
};
