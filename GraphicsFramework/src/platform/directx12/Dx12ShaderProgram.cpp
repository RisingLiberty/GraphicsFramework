#include "stdafx.h"

#include "Dx12ShaderProgram.h"
#include "Dx12HelperMethods.h"

Dx12ShaderProgram::Dx12ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc;
	descriptor_heap_desc.NumDescriptors = 2; // 1 for vertex shader, 1 for fragment shader
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // will be accessed by shaders
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; //a heap for constant buffers(shader resource views and unordered access views
	descriptor_heap_desc.NodeMask = 0;

	GetDx12Device()->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(m_constant_buffer_heap.ReleaseAndGetAddressOf()));
}

Dx12ShaderProgram::~Dx12ShaderProgram()
{

}

