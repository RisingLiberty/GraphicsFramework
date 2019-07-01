#pragma once

#include <graphics/VertexShader.h>

class OpenGLVertexShader : public VertexShader
{
public:
	OpenGLVertexShader(const std::string& path);
	~OpenGLVertexShader();

	void Bind() const override;
	void Unbind() const override;
	int Compile() override;

	unsigned int GetId() const;

private:
	unsigned int m_id;
};