#include "stdafx.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"

#include "scenegraph/SceneObject.h"

#include "Dx11Context.h"
#include "Dx11Renderer.h"
#include "Dx11HelperMethods.h"

Dx11Renderer::Dx11Renderer(ID3D11RenderTargetView* renderTargetView):
	m_render_target_view(renderTargetView)
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
		mesh->GetIndices()->Bind();

		ID3D11DeviceContext* device_context = dynamic_cast<Dx11Context*>(Context::GetCurrent())->GetDeviceContext();
		device_context->DrawIndexed((unsigned int)mesh->GetIndices()->GetCount(), 0, 0);
	}

	m_scene_objects.clear();
}

void Dx11Renderer::ClearAllBuffers()
{
	this->ClearColorBuffer();
}

void Dx11Renderer::ClearColorBuffer()
{
	GetDx11DeviceContext()->ClearRenderTargetView(m_render_target_view.Get(), &m_clear_color[0]);
}

void Dx11Renderer::ClearDepthStencilBuffer()
{
}

void Dx11Renderer::ClearDepthBuffer()
{
}

void Dx11Renderer::ClearStencilBuffer()
{
}
