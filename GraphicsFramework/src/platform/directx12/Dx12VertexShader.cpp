#include "stdafx.h"

#include "Dx12VertexShader.h"
#include "Dx12HelperMethods.h"
#include "Dx12Context.h"

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

Dx12VertexShader::Dx12VertexShader(const std::string& path) :
	VertexShader(path)
{
	this->Compile();
	m_code = LoadCode(m_path);
}

Dx12VertexShader::~Dx12VertexShader()
{

}

void Dx12VertexShader::Compile()
{
	UINT compiler_flags = 0;
#ifdef _DEBUG
	compiler_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

	ComPtr<ID3DBlob> errors;
	std::wstring wide_path(m_path.begin(), m_path.end());
	DXCALL(D3DCompileFromFile(wide_path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", compiler_flags, 0, m_compiled_code.ReleaseAndGetAddressOf(), errors.GetAddressOf()));

	if (errors)
		spdlog::error("error compiling shader: {}", (char*)errors->GetBufferPointer());
}

const std::string& Dx12VertexShader::GetCode() const
{
	return m_code;
}

D3D12_SHADER_BYTECODE Dx12VertexShader::GetByteCode() const
{
	return 	
	{
		reinterpret_cast<BYTE*>(m_compiled_code->GetBufferPointer()),
		m_compiled_code->GetBufferSize()
	};
}