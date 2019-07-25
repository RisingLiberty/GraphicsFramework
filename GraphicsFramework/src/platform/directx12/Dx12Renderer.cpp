#include "stdafx.h"

#include "Dx12Renderer.h"
#include "Dx12HelperMethods.h"
#include "Dx12Context.h"
#include "Dx12IndexBuffer.h"
#include "Dx12VertexArray.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "scenegraph/SceneObject.h"


Dx12Renderer::Dx12Renderer()
{
	unsigned int num_frame_resources = GetDx12Context()->GetNrOfFrameResources();
	DXGI_FORMAT back_buffer_format = GetDx12Context()->GetBackBufferFormat();
	ID3D12DescriptorHeap* srv_descriptor_heap = GetDx12Context()->GetSrvDescriptorHeap();

	ImGui_ImplDX12_Init(
		GetDx12Device(),
		1,
		back_buffer_format,
		srv_descriptor_heap->GetCPUDescriptorHandleForHeapStart(),
		srv_descriptor_heap->GetGPUDescriptorHandleForHeapStart());
}

Dx12Renderer::~Dx12Renderer()
{
	ImGui_ImplDX12_Shutdown();
}

void Dx12Renderer::Draw()
{
	for (SceneObject* object : m_scene_objects)
	{
		Mesh* mesh = object->GetMesh();
		Material* material = object->GetMaterial();

		material->Use();

		GetDx12Context()->BindResourcesToPipeline();
		GetDx12CommandList()->DrawIndexedInstanced((unsigned int)mesh->GetIndices()->GetCount(), 1, 0, 0, 0);
	}

	m_scene_objects.clear();
}

void Dx12Renderer::ClearAllBuffers()
{
	this->ClearColorBuffer();
	this->ClearDepthStencilBuffer();
}

void Dx12Renderer::ClearColorBuffer()
{
	GetDx12CommandList()->ClearRenderTargetView(GetDx12Context()->GetCurrentBackBufferView(), m_clear_color.data(), 0, nullptr);
}

void Dx12Renderer::ClearDepthStencilBuffer()
{
	GetDx12CommandList()->ClearDepthStencilView(GetDx12Context()->GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}

void Dx12Renderer::ClearDepthBuffer()
{
	GetDx12CommandList()->ClearDepthStencilView(GetDx12Context()->GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void Dx12Renderer::ClearStencilBuffer()
{
	GetDx12CommandList()->ClearDepthStencilView(GetDx12Context()->GetDepthStencilView(), D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}

void Dx12Renderer::Begin()
{
	GetDx12Context()->Begin();
}

void Dx12Renderer::End()
{
	GetDx12Context()->End();
}

void Dx12Renderer::RenderImgui()
{
	GetDx12Context()->BindImgui();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetDx12CommandList());
}