#include "stdafx.h"

#include "Dx11FragmentShader.h"
#include "Dx11HelperMethods.h"

Dx11FragmentShader::Dx11FragmentShader(const std::string& path):
	FragmentShader(path)
{
}


Dx11FragmentShader::~Dx11FragmentShader()
{
}

int Dx11FragmentShader::Compile()
{
	ID3DBlob* error_blob;
	DXCALL(D3DCompile(m_path.c_str(), m_path.size(), NULL, NULL, NULL, "PSMain", "ps_4_0", D3DCOMPILE_DEBUG, 0, &m_shader_copmiled_code, &error_blob));
	ASSERT(!error_blob, error_blob->GetBufferPointer());

	return S_OK;
}