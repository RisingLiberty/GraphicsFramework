#pragma once

#include "graphics/BufferHelpers.h"

class VertexBuffer;

class VertexBufferController
{
public:
	VertexBufferController();
	~VertexBufferController();

	VertexBuffer* Push(std::unique_ptr<VertexBuffer>& vb);
	VertexBuffer* Get(unsigned int ResourceId) const;
	VertexBuffer* Get(VertexBuffer* vb) const;
	VertexBuffer* Get(size_t size, BufferUsage usage) const;
private:
	std::vector<std::unique_ptr<VertexBuffer>> m_vertex_buffers;
};