#include "stdafx.h"

#include "VertexArrayController.h"

#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexLayout.h"

namespace
{
	size_t VertexArrayHash(const VertexBuffer* vb, const VertexLayout* layout)
	{
		__int64 a = (__int64)vb;
		__int64 b = (__int64)layout;

		//Cantor pairing function
		return (a + b) * (a + b + 1) / 2 + a;
	}

	size_t VertexArrayHash(VertexArray* vertexArray)
	{
		return VertexArrayHash(vertexArray->GetVertexBuffer(), vertexArray->GetVertexLayout());
	}
}

VertexArrayController::VertexArrayController() = default;
VertexArrayController::~VertexArrayController() = default;

void VertexArrayController::PushVertexArray(std::unique_ptr<VertexArray>& vertexArray)
{
	m_vertex_arrays[VertexArrayHash(vertexArray.get())] = (std::move(vertexArray));
}

VertexArray* VertexArrayController::GetVertexArray(const VertexBuffer* vb, const VertexLayout* layout)
{
	auto it = m_vertex_arrays.find(VertexArrayHash(vb, layout));

	if (it != m_vertex_arrays.cend())
		return it->second.get();
	else
		return nullptr;
}
