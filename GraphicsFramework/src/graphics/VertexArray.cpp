#include "stdafx.h"

#include "VertexArray.h"
#include "Context.h"

#include "controllers/VertexArrayController.h"

#include "platform/opengl/OpenGLVertexArray.h"
#include "platform/directx11/Dx11VertexArray.h"
#include "platform/directx12/Dx12VertexArray.h"
#include "platform/vulkan/VkVertexArray.h"

#include "platform/directx12/Dx12HelperMethods.h"
#include "platform/directx12/Dx12Context.h"

VertexArray::VertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	m_vertex_buffer(vb),
	m_vertex_layout(layout)
{

}

VertexArray::VertexArray(const unsigned int vbResourceId, const unsigned int ibResourceId, const unsigned int layoutResourceId) :
	m_vertex_buffer_resource_id(vbResourceId),
	m_index_buffer_resource_id(ibResourceId),
	m_vertex_layout_resource_id(layoutResourceId)
{

}

VertexArray::~VertexArray() = default;

VertexArray* VertexArray::Create(const VertexBuffer* vb, const VertexLayout* layout)
{
	VertexArrayController* vertex_array_controller = Context::GetCurrent()->GetVertexArrayController();
	VertexArray* vertex_array = vertex_array_controller->GetVertexArray(vb, layout);

	if (vertex_array)
		return vertex_array;

	std::unique_ptr<VertexArray> unique_array;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:
		unique_array = std::make_unique<OpenGLVertexArray>(vb, layout);
		vertex_array = unique_array.get();
		vertex_array_controller->PushVertexArray(unique_array);
		break;
	case Context::API::DIRECTX11:
		unique_array = std::make_unique<Dx11VertexArray>(vb, layout);
		vertex_array = unique_array.get();
		vertex_array_controller->PushVertexArray(unique_array);
		break;
	case Context::API::DIRECTX12:
		unique_array = std::make_unique<Dx12VertexArray>(vb, layout);
		vertex_array = unique_array.get();
		vertex_array_controller->PushVertexArray(unique_array);
		break;
	case Context::API::VULKAN:
		unique_array = std::make_unique<VkVertexArray>(vb, layout);
		vertex_array = unique_array.get();
		vertex_array_controller->PushVertexArray(unique_array);
	}

	return vertex_array;
}

const VertexBuffer* VertexArray::GetVertexBuffer() const
{
	return m_vertex_buffer;
}

const VertexLayout* VertexArray::GetVertexLayout() const
{
	return m_vertex_layout;
}

const VertexBuffer* VertexArray::GetVertexBufferObject() const
{
	return nullptr;
}

const IndexBuffer* VertexArray::GetIndexBufferObject() const
{
	return nullptr;
}

const VertexLayout* VertexArray::GetVertexLayoutObject() const
{
	return nullptr;
}