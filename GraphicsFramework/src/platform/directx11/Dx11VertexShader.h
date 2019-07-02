#pragma once

#include "graphics/VertexShader.h"

class Dx11VertexShader : public VertexShader
{
public:
	Dx11VertexShader(const std::string& path);
	virtual ~Dx11VertexShader();

protected:
	int Compile() override;

private:
	ID3DBlob* m_shader_compiled_code;
};

