#pragma once

#include "graphics/FragmentShader.h"

class Dx11FragmentShader : public FragmentShader
{
public:
	Dx11FragmentShader(const std::string& path);
	virtual ~Dx11FragmentShader();

	std::string GetCode() const;
	ID3DBlob* GetCompiledCode() const;
	ID3D11PixelShader* GetShader() const;

protected:
	virtual void Compile() override;

private:
	ComPtr<ID3DBlob> m_compiled_code;
	ComPtr<ID3D11PixelShader> m_shader;
	
	std::string m_code;
};

