#pragma once

#include "GraphicsResource.h"
#include "ShaderUniform.h"

class VertexShader;
class FragmentShader;

class ShaderProgram : public GraphicsResource<ShaderProgram>
{
public:
	ShaderProgram(VertexShader* vertexShader, FragmentShader* fragmentShader);
	virtual ~ShaderProgram();

	void SetUniform(const std::string& name, void* data, unsigned int size) const;

	void Bind() const;
	void Unbind() const;
	virtual void UploadVariables() const = 0;

	VertexShader* GetVertexShader() const;
	FragmentShader* GetFragmentShader() const;

	const std::vector<std::unique_ptr<ShaderUniform>>& GetUniforms() const;

	static ShaderProgram* Create(VertexShader* vs, FragmentShader* fs);

protected:
	VertexShader* m_vertex_shader;
	FragmentShader* m_fragment_shader;
	mutable std::vector<std::unique_ptr<ShaderUniform>> m_uniforms;
};

