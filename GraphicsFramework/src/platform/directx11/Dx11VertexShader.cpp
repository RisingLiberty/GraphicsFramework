#include "stdafx.h"

#include "Dx11VertexShader.h"
#include "Dx11HelperMethods.h"

Dx11VertexShader::Dx11VertexShader(const std::string& path):
	VertexShader(path)
{
}


Dx11VertexShader::~Dx11VertexShader()
{
}

int Dx11VertexShader::Compile()
{
	ID3DBlob* error_blob;
	DXCALL(D3DCompile(m_path.c_str(), m_path.size(), NULL, NULL, NULL, "VSMain", "vs_4_0", D3DCOMPILE_DEBUG, 0, &m_shader_compiled_code, &error_blob));
	ASSERT(!error_blob, error_blob->GetBufferPointer());

	return S_OK;
}