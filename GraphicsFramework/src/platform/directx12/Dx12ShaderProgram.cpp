#include "stdafx.h"

#include "Dx12ShaderProgram.h"
#include "Dx12HelperMethods.h"
#include "Dx12ShaderParser.h"
#include "Dx12VertexShader.h"
#include "Dx12FragmentShader.h"
#include "Dx12UploadBuffer.h"
#include "Dx12Context.h"
#include "Dx12ShaderUniform.h"

Dx12ShaderProgram::Dx12ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
	Dx12ShaderParser parser;

	Dx12VertexShader* dx_vs = GetDxVertexShader();
	Dx12FragmentShader* dx_fs = GetDxFragmentShader();

	Dx12ParsedShader vs_parsed = parser.Parse(dx_vs->GetCode());
	Dx12ParsedShader fs_parsed = parser.Parse(dx_fs->GetCode());

	//temp for testing
	if (!fs_parsed.buffers.empty())
	{
		m_constant_buffer = std::make_unique<Dx12UploadBuffer>(fs_parsed.buffers.front());
		for (const std::unique_ptr<Dx12ShaderUniform>& uniform : fs_parsed.buffers[0].uniforms)
			m_uniforms.emplace_back(std::make_unique<Dx12ShaderUniform>(uniform->name, uniform->type, uniform->offset, uniform->size));
	}

	this->BuildRootSignature(&vs_parsed, &fs_parsed);
}

Dx12ShaderProgram::~Dx12ShaderProgram()
{

}

void Dx12ShaderProgram::Bind() const
{
	GetDx12Context()->BindShaderProgram(this);
}

void Dx12ShaderProgram::Unbind() const
{
	GetDx12Context()->UnbindShaderProgram(this);
}

void Dx12ShaderProgram::UploadVariables() const
{
	for (const std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
	{
		const Dx12ShaderUniform* dx_uniform = static_cast<const Dx12ShaderUniform*>(uniform.get());
		m_constant_buffer->CopyData(dx_uniform->data, dx_uniform->size, dx_uniform->offset);
	}
}

Dx12UploadBuffer* Dx12ShaderProgram::GetUploadBuffer() const
{
	return m_constant_buffer.get();
}

ID3D12RootSignature* Dx12ShaderProgram::GetRootSignature() const
{
	return m_root_signature.Get();
}

void Dx12ShaderProgram::BuildRootSignature(Dx12ParsedShader* vs_parsed, Dx12ParsedShader* fs_parsed)
{
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  

	// Root parameter can be a table, root descriptor or root constants.
	std::vector<CD3DX12_ROOT_PARAMETER> root_parameters;

	spdlog::warn("Root signatures creation only includes constant buffers!");

	for (auto buffer : vs_parsed->buffers)
	{
		CD3DX12_DESCRIPTOR_RANGE cbv_table;
		cbv_table.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		CD3DX12_ROOT_PARAMETER param;
		param.InitAsDescriptorTable(1, &cbv_table);
		root_parameters.push_back(param);
	}

	for (auto buffer : fs_parsed->buffers)
	{
		CD3DX12_DESCRIPTOR_RANGE cbv_table;
		cbv_table.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		CD3DX12_ROOT_PARAMETER param;
		param.InitAsDescriptorTable(1, &cbv_table);
		root_parameters.push_back(param);
	}

	CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc((unsigned int)root_parameters.size(), root_parameters.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> serialized_root_signature;
	ComPtr<ID3DBlob> errors;
	DXCALL(D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, serialized_root_signature.GetAddressOf(), errors.GetAddressOf()));

	if (errors)
		spdlog::error((char*)errors->GetBufferPointer());

	DXCALL(GetDx12Device()->CreateRootSignature(
		0,
		serialized_root_signature->GetBufferPointer(),
		serialized_root_signature->GetBufferSize(),
		IID_PPV_ARGS(m_root_signature.ReleaseAndGetAddressOf())));
}

Dx12VertexShader* Dx12ShaderProgram::GetDxVertexShader() const
{
	return static_cast<Dx12VertexShader*>(m_vertex_shader);
}

Dx12FragmentShader* Dx12ShaderProgram::GetDxFragmentShader() const
{
	return static_cast<Dx12FragmentShader*>(m_fragment_shader);
}
