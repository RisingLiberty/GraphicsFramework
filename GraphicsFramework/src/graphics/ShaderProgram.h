#pragma once

#include "ShaderUniform.h"

class VertexShader;
class FragmentShader;

class ShaderProgram
{
public:
	ShaderProgram(VertexShader* vertexShader, FragmentShader* fragmentShader);
	virtual ~ShaderProgram();

	void SetUniform(const std::string& name, void* data, unsigned int size);

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void UploadVariables() = 0;

	VertexShader* GetVertexShader() const;
	FragmentShader* GetFragmentShader() const;

	const std::vector<std::unique_ptr<ShaderUniform>>& GetUniforms() const;

	static ShaderProgram* Create(VertexShader* vs, FragmentShader* fs);

protected:
	VertexShader* m_vertex_shader;
	FragmentShader* m_fragment_shader;
	std::vector<std::unique_ptr<ShaderUniform>> m_uniforms;
};

