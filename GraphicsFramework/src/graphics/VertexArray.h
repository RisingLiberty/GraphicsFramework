#pragma once

#include "GraphicsResource.h"

class VertexBuffer;
class VertexLayout;
class IndexBuffer;

class VertexArray : public GraphicsResource<VertexArray>
{
public:
	virtual ~VertexArray();

	void Bind() const;
	void Unbind() const;

	const VertexBuffer* GetVertexBuffer() const;
	const VertexLayout* GetVertexLayout() const;

	static VertexArray* Create(const VertexBuffer* vb, const VertexLayout* layout);

protected:
	VertexArray(const VertexBuffer* vb, const VertexLayout* layout);
	VertexArray(const unsigned int vbResourceId, const unsigned int ibResourceId, const unsigned int layoutResourceId);

	const VertexBuffer* GetVertexBufferObject() const;
	const IndexBuffer* GetIndexBufferObject() const;
	const VertexLayout* GetVertexLayoutObject() const;

protected:
	unsigned int m_vertex_buffer_resource_id;
	unsigned int m_index_buffer_resource_id;
	unsigned int m_vertex_layout_resource_id;



	const VertexBuffer* m_vertex_buffer;
	const VertexLayout* m_vertex_layout;
};