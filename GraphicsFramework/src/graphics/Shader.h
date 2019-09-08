#pragma once

class Shader
{
public:
	enum class Type
	{
		VERTEX,
		FRAGMENT
	};

	Shader(const std::string& path);
	virtual ~Shader();

	const std::string& GetPath() const;
	virtual Type GetType() const = 0;
protected:
	virtual void Compile() = 0;

protected:
	std::string m_path;
};
