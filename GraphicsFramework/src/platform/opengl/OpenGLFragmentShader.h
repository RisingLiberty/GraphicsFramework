#pragma once

#include <graphics/FragmentShader.h>

class OpenGLFragmentShader : public FragmentShader
{
public:
	OpenGLFragmentShader(const std::string& path);
	~OpenGLFragmentShader();

	void Bind() const override;
	void Unbind() const override;

	unsigned int GetId() const;

protected:
	int Compile() override;

private:
	unsigned int m_id;
};