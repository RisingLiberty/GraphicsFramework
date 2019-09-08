#pragma once

#include "graphics/VertexShader.h"

class Dx12VertexShader : public VertexShader
{
public:
	Dx12VertexShader(const std::string& name);
	virtual ~Dx12VertexShader();

	const std::string& GetCode() const;
	D3D12_SHADER_BYTECODE GetByteCode() const;

protected:
	virtual void Compile() override;

private:
	ComPtr<ID3DBlob> m_compiled_code;
	std::string m_code;
};