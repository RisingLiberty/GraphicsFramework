#pragma once

class VertexArray;
class VertexBuffer;
class VertexLayout;

class VertexArrayController
{
public:
	VertexArrayController();
	~VertexArrayController();

	void PushVertexArray(std::unique_ptr<VertexArray>& vertexArray);
	VertexArray* GetVertexArray(const VertexBuffer* vb, const VertexLayout* layout);

private:
	std::map<size_t, std::unique_ptr<VertexArray>> m_vertex_arrays;
};