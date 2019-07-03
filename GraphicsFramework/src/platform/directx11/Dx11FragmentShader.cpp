#include "stdafx.h"

#include "Dx11FragmentShader.h"
#include "Dx11HelperMethods.h"
#include "Dx11Context.h"

Dx11FragmentShader::Dx11FragmentShader(const std::string& path):
	FragmentShader(path)
{
	this->Compile();
}


Dx11FragmentShader::~Dx11FragmentShader()
{
}

int Dx11FragmentShader::Compile()
{
	unsigned int compile_flags = 0;
#if defined _DEBUG
	compile_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	std::wstring w_path(m_path.begin(), m_path.end());

	ComPtr<ID3DBlob> error_blob;
	DXCALL(D3DCompileFromFile(w_path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_4_0", compile_flags, 0, m_shader_compiled_code.ReleaseAndGetAddressOf(), error_blob.GetAddressOf()));
	ASSERT(!error_blob, (char*)error_blob->GetBufferPointer());

	DXCALL(GetDx11Context()->GetDevice()->CreatePixelShader(m_shader_compiled_code->GetBufferPointer(), m_shader_compiled_code->GetBufferSize(), NULL, m_shader.ReleaseAndGetAddressOf()));

	return S_OK;
}