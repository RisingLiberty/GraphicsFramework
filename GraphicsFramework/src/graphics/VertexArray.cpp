#include "stdafx.h"

#include "VertexArray.h"
#include "Context.h"

#include "controllers/VertexArrayController.h"

#include "platform/opengl/OpenGLVertexArray.h"
#include "platform/directx11/Dx11VertexArray.h"

VertexArray::VertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	m_vertex_buffer(vb),
	m_vertex_layout(layout)
{

}

VertexArray::~VertexArray() = default;

VertexArray* VertexArray::Create(const VertexBuffer* vb, const VertexLayout* layout)
{
	VertexArrayController* vertex_array_controller = Context::GetCurrent()->GetVertexArrayController();
	VertexArray* vertex_array = vertex_array_controller->GetVertexArray(vb, layout);

	if (vertex_array)
		return vertex_array;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:
	{
		std::unique_ptr<VertexArray> unique_array = std::make_unique<OpenGLVertexArray>(vb, layout);
		vertex_array = unique_array.get();
		vertex_array_controller->PushVertexArray(unique_array);
		break;
	}
	case Context::API::DIRECTX11:
		std::unique_ptr<VertexArray> unique_array = std::make_unique<Dx11VertexArray>(vb, layout);
		vertex_array = unique_array.get();
		vertex_array_controller->PushVertexArray(unique_array);
		break;
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