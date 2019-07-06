#include "stdafx.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"

#include "scenegraph/SceneObject.h"

#include "Dx11Context.h"
#include "Dx11Renderer.h"
#include "Dx11HelperMethods.h"

#include "Dx11VertexBuffer.h"
#include "Dx11VertexLayout.h"

Dx11Renderer::Dx11Renderer(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView) :
	m_render_target_view(renderTargetView),
	m_depth_stencil_view(depthStencilView)
{
	memcpy(m_clear_color.data(), &DirectX::Colors::Red, sizeof(float) * 4);
	ImGui_ImplDX11_Init(GetDx11Device(), GetDx11DeviceContext());
}

Dx11Renderer::~Dx11Renderer()
{
	ImGui_ImplDX11_Shutdown();
}

void Dx11Renderer::Present()
{
	for (SceneObject* object : m_scene_objects)
	{
		Mesh* mesh = object->GetMesh();
		Material* material = object->GetMaterial();

		material->Use();

		VertexArray* va = VertexArray::Create(mesh->GetVertices(), mesh->GetVertexLayout());
		va->Bind();
		mesh->GetIndices()->Bind();

		GetDx11DeviceContext()->DrawIndexed((unsigned int)mesh->GetIndices()->GetCount(), 0, 0);
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
	GetDx11DeviceContext()->ClearRenderTargetView(m_render_target_view.Get(), &m_clear_color[0]);
}

void Dx11Renderer::ClearDepthStencilBuffer()
{
	GetDx11DeviceContext()->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Dx11Renderer::ClearDepthBuffer()
{
	GetDx11DeviceContext()->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Dx11Renderer::ClearStencilBuffer()
{
	GetDx11DeviceContext()->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Dx11Renderer::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}