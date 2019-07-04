#pragma once

#include "graphics/FragmentShader.h"

class Dx12FragmentShader : public FragmentShader
{
public:
	Dx12FragmentShader(const std::string& path);
	virtual ~Dx12FragmentShader();
	const std::string& GetCode() const;
	ID3DBlob* GetCompiledCode() const;

protected:
	int Compile() override;
private:
	ComPtr<ID3DBlob> m_compiled_code;
	std::string m_code;
};