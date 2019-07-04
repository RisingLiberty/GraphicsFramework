#include "stdafx.h"

#include "Dx12ShaderProgram.h"
#include "Dx12HelperMethods.h"
#include "Dx12ShaderParser.h"
#include "Dx12VertexShader.h"
#include "Dx12FragmentShader.h"
#include "Dx12UploadBuffer.h"
#include "Dx12Context.h"

Dx12ShaderProgram::Dx12ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc;
	descriptor_heap_desc.NumDescriptors = 1; // just 1 for now, the one in the fragment shader
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // will be accessed by shaders
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; //a heap for constant buffers(shader resource views and unordered access views
	descriptor_heap_desc.NodeMask = 0;

	GetDx12Device()->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(m_constant_buffer_heap.ReleaseAndGetAddressOf()));
	Dx12ShaderParser parser;

	Dx12VertexShader* dx_vs = GetDxVertexShader();
	Dx12FragmentShader* dx_fs = GetDxFragmentShader();

	Dx12ParsedShader vs_parsed = parser.Parse(dx_vs->GetCode());
	Dx12ParsedShader fs_parsed = parser.Parse(dx_fs->GetCode());

	//temp for testing
	if (!fs_parsed.buffers.empty())
	{
		auto upload_buffer = std::make_unique<Dx12UploadBuffer>(fs_parsed.buffers.front());

		unsigned int buffer_size = fs_parsed.buffers.front().real_size;
		D3D12_GPU_VIRTUAL_ADDRESS gpu_address = upload_buffer->GetResource()->GetGPUVirtualAddress();

		// this address varies per object, per buffer.
		// for now the returned should suffice
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc;
		cbv_desc.BufferLocation = gpu_address;
		cbv_desc.SizeInBytes = buffer_size;

		GetDx12Device()->CreateConstantBufferView(&cbv_desc, m_constant_buffer_heap->GetCPUDescriptorHandleForHeapStart());
	}

	CD3DX12_ROOT_PARAMETER slot_root_parameter[1];

	CD3DX12_DESCRIPTOR_RANGE cbv_table;
	cbv_table.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slot_root_parameter[0].InitAsDescriptorTable(1, &cbv_table);

	CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc(1, slot_root_parameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serialized_root_signature;
	ComPtr<ID3DBlob> errors;
	DXCALL(D3D12SerializeRootSignature(
		&root_signature_desc, 
		D3D_ROOT_SIGNATURE_VERSION_1, 
		serialized_root_signature.GetAddressOf(), 
		errors.GetAddressOf()));

	if (errors)
		spdlog::error("Error when creating root signature!\n{}", (char*)errors->GetBufferPointer());

	DXCALL(GetDx12Device()->CreateRootSignature(
		0, 
		serialized_root_signature->GetBufferPointer(), 
		serialized_root_signature->GetBufferSize(), 
		IID_PPV_ARGS(m_root_signature.ReleaseAndGetAddressOf())));

	GetDx12Context()->BindRootSignature(m_root_signature.Get());
}

Dx12ShaderProgram::~Dx12ShaderProgram()
{

}

void Dx12ShaderProgram::Bind() const
{
}

void Dx12ShaderProgram::Unbind() const
{
}

void Dx12ShaderProgram::UploadVariables()
{
}

Dx12VertexShader* Dx12ShaderProgram::GetDxVertexShader() const
{
	return dynamic_cast<Dx12VertexShader*>(m_vertex_shader);
}

Dx12FragmentShader* Dx12ShaderProgram::GetDxFragmentShader() const
{
	return dynamic_cast<Dx12FragmentShader*>(m_fragment_shader);
}
