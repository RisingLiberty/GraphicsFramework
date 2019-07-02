#pragma once

#include <graphics/VertexShader.h>

class OpenGLVertexShader : public VertexShader
{
public:
	OpenGLVertexShader(const std::string& path);
	~OpenGLVertexShader();

	unsigned int GetId() const;

protected:
	int Compile() override;

private:
	unsigned int m_id;
};