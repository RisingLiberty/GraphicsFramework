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
	m_clear_color = DirectX::Colors::Red;
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

		Dx11VertexBuffer* dx_vb = dynamic_cast<Dx11VertexBuffer*>(mesh->GetVertices());
		D3D11_BUFFER_DESC vb_desc;
		dx_vb->GetBuffer()->GetDesc(&vb_desc);

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = vb_desc.ByteWidth;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(float);

		ComPtr<ID3D11Buffer> stagging_buffer;
		GetDx11Device()->CreateBuffer(&desc, NULL, stagging_buffer.GetAddressOf());
		GetDx11DeviceContext()->CopyResource(stagging_buffer.Get(), dx_vb->GetBuffer());
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDx11DeviceContext()->Map(stagging_buffer.Get(), 0, D3D11_MAP_READ, 0, &msr);
		GetDx11DeviceContext()->Unmap(stagging_buffer.Get(), 0);

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
