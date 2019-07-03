#include "stdafx.h"

#include "Dx11VertexShader.h"
#include "Dx11HelperMethods.h"
#include "Dx11Context.h"

namespace
{
	std::string LoadCode(const std::string& path)
	{
		std::fstream file(path, std::ios::in | std::ios::ate);

		if (!file.is_open())
		{
			spdlog::warn("Failed to open shader at {}", path);
			return "";
		}

		size_t size = file.tellg();

		std::vector<char> content(size);
		file.seekg(0);

		file.read(content.data(), size);

		return content.data();
	}
}


Dx11VertexShader::Dx11VertexShader(const std::string& path):
	VertexShader(path)
{
	this->Compile();
	m_code = LoadCode(path);
}


Dx11VertexShader::~Dx11VertexShader()
{
}

std::string Dx11VertexShader::GetCode() const
{
	return m_code;
}

ID3D11VertexShader* Dx11VertexShader::GetShader() const
{
	return m_shader.Get();
}

int Dx11VertexShader::Compile()
{
	unsigned int compile_flags = 0;
#if defined _DEBUG
	compile_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	std::wstring w_path(m_path.begin(), m_path.end());

	ComPtr<ID3DBlob> error_blob;
	DXCALL(D3DCompileFromFile(w_path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_4_0", compile_flags, 0, &m_shader_compiled_code, error_blob.GetAddressOf()));
	ASSERT(!error_blob, (char*)error_blob->GetBufferPointer());

	DXCALL(GetDx11Context()->GetDevice()->CreateVertexShader(m_shader_compiled_code->GetBufferPointer(), m_shader_compiled_code->GetBufferSize(), NULL, m_shader.ReleaseAndGetAddressOf()));

	return S_OK;
}