#pragma once

#include "graphics/FragmentShader.h"

class Dx12FragmentShader : public FragmentShader
{
public:
	Dx12FragmentShader(const std::string& path);
	virtual ~Dx12FragmentShader();
	
	const std::string& GetCode() const;
	D3D12_SHADER_BYTECODE GetByteCode() const;

protected:
	virtual void Compile() override;
private:
	ComPtr<ID3DBlob> m_compiled_code;
	std::string m_code;
};