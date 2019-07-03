#pragma once

#include "graphics/FragmentShader.h"

class Dx11FragmentShader : public FragmentShader
{
public:
	Dx11FragmentShader(const std::string& path);
	virtual ~Dx11FragmentShader();

protected:
	int Compile() override;

private:
	ComPtr<ID3DBlob> m_shader_compiled_code;
	ComPtr<ID3D11PixelShader> m_shader;

};

