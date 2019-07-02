#pragma once

class VertexShader;
class FragmentShader;
class ShaderProgram;

class ShaderController
{
public:
	ShaderController();
	~ShaderController();

	void PushVertexShader(std::unique_ptr<VertexShader>& vertexShader);
	void PushFragmentShader(std::unique_ptr<FragmentShader>& fragmentShader);
	void PushShaderProgram(std::unique_ptr<ShaderProgram>& shaderProgram);

	VertexShader* GetVertexShader(const std::string& path) const;
	FragmentShader* GetFragmentShader(const std::string& path) const;
	ShaderProgram* GetShaderProgram(VertexShader* vs, FragmentShader* fs) const;

private:
	std::unordered_map<std::string, std::unique_ptr<VertexShader>> m_vertex_shaders;
	std::unordered_map<std::string, std::unique_ptr<FragmentShader>> m_fragment_shaders;
	std::unordered_map<size_t, std::unique_ptr<ShaderProgram>> m_shader_programs;
};