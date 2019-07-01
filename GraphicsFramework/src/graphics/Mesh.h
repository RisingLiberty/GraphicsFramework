#pragma once

#include "Vertex.h"

class VertexBuffer;
class VertexLayout;
class IndexBuffer;

class Mesh
{
public:
	Mesh(std::unique_ptr<VertexBuffer>& vb, std::unique_ptr<VertexLayout>& layout, std::unique_ptr<IndexBuffer>& ib);
	Mesh(const std::string& path);
	~Mesh();

	VertexBuffer* GetVertices() const;
	VertexLayout* GetVertexLayout() const;
	IndexBuffer* GetIndices() const;

private:
	std::unique_ptr<VertexBuffer> m_vertices;
	std::unique_ptr<VertexLayout> m_vertex_layout;
	std::unique_ptr<IndexBuffer> m_indices;
};