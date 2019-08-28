#pragma once

#include <graphics/FragmentShader.h>

class GLFragmentShader : public FragmentShader
{
public:
	GLFragmentShader(const std::string& path);
	~GLFragmentShader();

	unsigned int GetId() const;

protected:
	int Compile() override;

private:
	unsigned int m_id;
};