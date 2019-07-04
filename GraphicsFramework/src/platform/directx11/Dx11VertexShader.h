#pragma once

#include "graphics/VertexShader.h"

class Dx11VertexShader : public VertexShader
{
public:
	Dx11VertexShader(const std::string& path);
	virtual ~Dx11VertexShader();

	const std::string& GetCode() const;
	ID3DBlob* GetCompiledCode() const;
	ID3D11VertexShader* GetShader() const;

protected:
	int Compile() override;

private:
	ComPtr<ID3DBlob> m_compiled_code;
	ComPtr<ID3D11VertexShader> m_shader;

	std::string m_code;
};

