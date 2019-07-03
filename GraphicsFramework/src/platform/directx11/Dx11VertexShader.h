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
	ComPtr<ID3DBlob> m_shader_compiled_code;
	ComPtr<ID3D11VertexShader> m_shader;


};

