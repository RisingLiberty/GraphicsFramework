#pragma once

class Shader
{
public:
	Shader(const std::string& path);
	virtual ~Shader();

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual int Compile() = 0;

protected:
	std::string m_path;
};