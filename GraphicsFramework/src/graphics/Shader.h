#pragma once

class Shader
{
public:
	Shader(const std::string& path);
	virtual ~Shader();

	const std::string& GetPath() const;

protected:
	virtual int Compile() = 0;

protected:
	std::string m_path;
};
