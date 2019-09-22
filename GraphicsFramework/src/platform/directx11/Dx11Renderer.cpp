#include "stdafx.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"

#include "scenegraph/SceneObject.h"

#include "Dx11Context.h"
#include "Dx11Renderer.h"
#include "Dx11HelperMethods.h"

#include "Dx11CommandList.h"
#include "Dx11VertexBuffer.h"
#include "Dx11VertexLayout.h"

#include "commands/Dx11DrawIndexedCommand.h"
#include "commands/Dx11ClearDepthBufferCommand.h"
#include "commands/Dx11ClearDepthStencilBufferCommand.h"
#include "commands/Dx11ClearRenderTargetCommand.h"
#include "commands/Dx11ClearStencilBufferCommand.h"

Dx11Renderer::Dx11Renderer(ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView) :
	m_render_target_view(renderTargetView),
	m_depth_stencil_view(depthStencilView)
{
	memcpy(m_clear_color.data(), &DirectX::Colors::Red, sizeof(float) * 4);
	ImGui_ImplDX11_Init(GetDx11Device(), deviceContext);
}

Dx11Renderer::~Dx11Renderer()
{
	ImGui_ImplDX11_Shutdown();
}

void Dx11Renderer::Draw()
{
	for (SceneObject* object : m_scene_objects)
	{
		Mesh* mesh = object->GetMesh();
		Material* material = object->GetMaterial();

		material->Use();

		mesh->GetVertexArray()->Bind();
		mesh->GetIndices()->Bind();

		GetDx11CommandList()->Push<Dx11DrawIndexedCommand>(mesh->GetIndices()->GetCount());
	}

	m_scene_objects.clear();
}

void Dx11Renderer::ClearAllBuffers()
{
	this->ClearColorBuffer();
	this->ClearDepthStencilBuffer();
}

void Dx11Renderer::ClearColorBuffer()
{
	GetDx11CommandList()->Push<Dx11ClearRenderTargetCommand>(m_clear_color, m_render_target_view.Get());
}

void Dx11Renderer::ClearDepthStencilBuffer()
{
	GetDx11CommandList()->Push<Dx11ClearDepthStencilBufferCommand>(1.0f, 0, m_depth_stencil_view.Get());
}

void Dx11Renderer::ClearDepthBuffer()
{
	GetDx11CommandList()->Push<Dx11ClearDepthBufferCommand>(1.0f, m_depth_stencil_view.Get());
}

void Dx11Renderer::ClearStencilBuffer()
{
	GetDx11CommandList()->Push<Dx11ClearStencilBufferCommand>(0, m_depth_stencil_view.Get());
}

void Dx11Renderer::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}