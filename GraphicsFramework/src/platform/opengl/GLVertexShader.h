#pragma once

#include <graphics/VertexShader.h>

class GLVertexShader : public VertexShader
{
public:
	GLVertexShader(const std::string& path);
	~GLVertexShader();

	unsigned int GetId() const;

protected:
	void Compile() override;

private:
	unsigned int m_id;
};