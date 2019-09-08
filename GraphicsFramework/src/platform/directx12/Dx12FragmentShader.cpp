#include "stdafx.h"

#include "Dx12FragmentShader.h"
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

Dx12FragmentShader::Dx12FragmentShader(const std::string& path) :
	FragmentShader(path)
{
	this->Compile();
	m_code = LoadCode(m_path);
}

Dx12FragmentShader::~Dx12FragmentShader()
{

}

void Dx12FragmentShader::Compile()
{
	UINT compiler_flags = 0;
#ifdef _DEBUG
	compiler_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

	ComPtr<ID3DBlob> errors;
	std::wstring wide_code(m_path.begin(), m_path.end());
	DXCALL(D3DCompileFromFile(wide_code.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", compiler_flags, 0, m_compiled_code.ReleaseAndGetAddressOf(), errors.GetAddressOf()));

	if (errors)
		spdlog::error("error compiling shader: {}", (char*)errors->GetBufferPointer());
}

const std::string& Dx12FragmentShader::GetCode() const
{
	return m_code;
}

D3D12_SHADER_BYTECODE Dx12FragmentShader::GetByteCode() const
{
	return
	{
		reinterpret_cast<BYTE*>(m_compiled_code->GetBufferPointer()),
		m_compiled_code->GetBufferSize()
	};
}