#include "stdafx.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"

#include "scenegraph/SceneObject.h"

#include "Dx11Context.h"
#include "Dx11Renderer.h"

Dx11Renderer::Dx11Renderer()
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

		float color[] = { 1.0f, 0.3f, 0.8f, 1.0f };
		material->SetParameter("u_Color", color, sizeof(color));
		material->Use();

		VertexArray* va = VertexArray::Create(mesh->GetVertices(), mesh->GetVertexLayout());
		mesh->GetIndices()->Bind();

		ID3D11DeviceContext* device_context = dynamic_cast<Dx11Context*>(Context::GetCurrent())->GetDeviceContext();
		device_context->DrawIndexed(mesh->GetIndices()->GetCount(), 0, 0);
	}
}

void Dx11Renderer::ClearAllBuffers()
{
}

void Dx11Renderer::ClearColorBuffer()
{
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
