#pragma once

#include "ShaderUniform.h"

class VertexShader;
class FragmentShader;

class ShaderProgram
{
public:
	ShaderProgram(VertexShader* vertexShader, FragmentShader* fragmentShader);
	virtual ~ShaderProgram();

	virtual void SetBoolUniform(const std::string& name,  bool uniform) = 0;
	virtual void SetUShortUniform(const std::string& name,  unsigned short& uniform) = 0;
	virtual void SetShortUniform(const std::string& name,  short uniform) = 0;
	virtual void SetIntUniform(const std::string& name,  int uniform) = 0;
	virtual void SetUIntUniform(const std::string& name,  unsigned int& uniform) = 0;
	virtual void SetFloatUniform(const std::string& name,  float uniform) = 0;
	virtual void SetDoubleUniform(const std::string& name,  double uniform) = 0;
	virtual void SetVec2Uniform(const std::string& name, float x, float y) = 0;
	virtual void SetVec2Uniform(const std::string& name, float* values) = 0;
	virtual void SetVec3Uniform(const std::string& name, float x, float y, float z) = 0;
	virtual void SetVec3Uniform(const std::string& name, float* values) = 0;
	virtual void SetVec4Uniform(const std::string& name, float x, float y, float z, float w) = 0;
	virtual void SetVec4Uniform(const std::string& name, float* values) = 0;
	virtual void SetMat3Uniform(const std::string& name, float* values) = 0;
	virtual void SetMat4Uniform(const std::string& name, float* values) = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	VertexShader* GetVertexShader() const;
	FragmentShader* GetFragmentShader() const;

	virtual const std::vector<std::unique_ptr<ShaderUniform>>& GetUniforms() const = 0;

	static ShaderProgram* Create(VertexShader* vs, FragmentShader* fs);

private:
	VertexShader* m_vertex_shader;
	FragmentShader* m_fragment_shader;
};

