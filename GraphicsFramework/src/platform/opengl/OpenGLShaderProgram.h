#pragma once

#include "HelperMethods.h"

#include <graphics/ShaderUniform.h>
#include <graphics/ShaderProgram.h>

#include <GL/glew.h>

class OpenGLShaderProgram : public ShaderProgram
{
public:
	OpenGLShaderProgram();
	virtual ~OpenGLShaderProgram();

	void Create(const std::vector<unsigned int>& shaders);

	void Bind() const;
	void UnBind() const;

	unsigned int GetId() const { return m_id; }

	template <typename T>
	void SetUniform(const std::string& name, const T& uniform)
	{
		assert(false && "SetUniform method not overriden with correct method yet");
	}

	template <>
	void SetUniform<float>(const std::string& name, const float& uniform)
	{
		GLCALL(glUniform1f(GetUniformLocation(name), uniform));
	}

	template <>
	void SetUniform<int>(const std::string& name, const int& uniform)
	{
		GLCALL(glUniform1i(GetUniformLocation(name), uniform));
	}

	void SetVector4Uniform(const std::string& name, float x, float y, float z, float w)
	{
		GLCALL(glUniform4f(GetUniformLocation(name), x, y, z, w));
	}

	//template <>
	//void SetUniform<Math::Vec2>(const std::string& name, const Math::Vec2& vec)
	//{
	//	GLCALL(glUniform2f(GetUniformLocation(name), vec.x, vec.y));
	//}

	//template <>
	//void SetUniform<Math::Vec3>(const std::string& name, const Math::Vec3& vec)
	//{
	//	GLCALL(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
	//}

	//template <>
	//void SetUniform<Math::Vec4>(const std::string& name, const Math::Vec4& vec)
	//{
	//	GLCALL(glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
	//}

	//template <>
	//void SetUniform<Math::Mat44>(const std::string& name, const Math::Mat44& mat)
	//{
	//	GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat.m[0][0]));
	//}

	int GetUniformLocation(const std::string& name);

	std::vector<ShaderUniform> GetUniforms() const;

private:

private:
	unsigned int m_id;
	std::map<std::string, int> m_UniformLocationCache;
};
