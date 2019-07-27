#pragma once

class VertexArray;
class VertexBuffer;
class VertexLayout;
class IndexBuffer;

class Mesh
{
public:
	Mesh(VertexBuffer* vb, VertexLayout* layout, IndexBuffer* ib);
	//Mesh(const std::string& path);
	~Mesh();

	const VertexArray* GetVertexArray() const;
	const VertexBuffer* GetVertices() const;
	const VertexLayout* GetVertexLayout() const;
	const IndexBuffer* GetIndices() const;

private:
	
	VertexArray* m_vertex_array;
	IndexBuffer* m_indices;
};