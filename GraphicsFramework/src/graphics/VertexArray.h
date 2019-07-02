#pragma once

class VertexBuffer;
class VertexLayout;

class VertexArray
{
public:
	virtual ~VertexArray();

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	const VertexBuffer* GetVertexBuffer() const;
	const VertexLayout* GetVertexLayout() const;

	static VertexArray* Create(const VertexBuffer* vb, const VertexLayout* layout);

protected:
	VertexArray(const VertexBuffer* vb, const VertexLayout* layout);

protected:
	const VertexBuffer* m_vertex_buffer;
	const VertexLayout* m_vertex_layout;
};