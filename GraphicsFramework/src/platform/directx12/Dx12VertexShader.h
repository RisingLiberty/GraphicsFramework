#pragma once

#include "graphics/VertexShader.h"

class Dx12VertexShader : public VertexShader
{
public:
	Dx12VertexShader(const std::string& name);
	virtual ~Dx12VertexShader();

	const std::string& GetCode() const;
	ID3DBlob* GetCompiledCode() const;

protected:
	int Compile() override;

private:
	ComPtr<ID3DBlob> m_compiled_code;
	std::string m_code;
};