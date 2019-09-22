#include "stdafx.h"

#include "Dx12Renderer.h"
#include "Dx12HelperMethods.h"
#include "Dx12Context.h"
#include "Dx12IndexBuffer.h"
#include "Dx12VertexArray.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "scenegraph/SceneObject.h"

#include "Dx12CommandList.h"
#include "Dx12CommandQueue.h"

#include "commands/Dx12DrawIndexedCommand.h"

#include "commands/Dx12ClearRenderTargetCommand.h"
#include "commands/Dx12ClearDepthBufferCommand.h"
#include "commands/Dx12ClearDepthStencilBufferCommand.h"
#include "commands/Dx12ClearStencilBufferCommand.h"

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

		mesh->GetVertexArray()->Bind();
		mesh->GetIndices()->Bind();

		GetDx12Context()->BindResourcesToPipeline();
        GetDx12CommandList()->Push<Dx12DrawIndexedCommand>(mesh->GetIndices()->GetCount());
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
    auto direct_cmd_list = GetDx12Context()->CreateDirectCommandList();
    direct_cmd_list->Push<Dx12ClearRenderTargetCommand>(m_clear_color, GetDx12Context()->GetCurrentBackBufferView());
}

void Dx12Renderer::ClearDepthStencilBuffer()
{
    auto direct_cmd_list = GetDx12Context()->CreateDirectCommandList();
    direct_cmd_list->Push<Dx12ClearDepthStencilBufferCommand>(1.0f, 0, GetDx12Context()->GetDepthStencilView());
}

void Dx12Renderer::ClearDepthBuffer()
{
    auto direct_cmd_list = GetDx12Context()->CreateDirectCommandList();
    direct_cmd_list->Push<Dx12ClearDepthBufferCommand>(1.0f, GetDx12Context()->GetDepthStencilView());
}

void Dx12Renderer::ClearStencilBuffer()
{
    auto direct_cmd_list = GetDx12Context()->CreateDirectCommandList();
    direct_cmd_list->Push<Dx12ClearStencilBufferCommand>(0, GetDx12Context()->GetDepthStencilView());
}

void Dx12Renderer::RenderImgui()
{
	GetDx12CommandList()->Open();
	GetDx12Context()->BindImgui();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetDx12CommandList()->GetApiCommandList());
	GetDx12CommandList()->Close();  
    
    GetDx12CommandQueue()->Execute(GetDx12CommandList());
    GetDx12CommandQueue()->Flush();

}