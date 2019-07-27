#pragma once

class IndexBuffer;

class IndexBufferController
{
public:
	IndexBufferController();
	~IndexBufferController();

	IndexBuffer* Push(std::unique_ptr<IndexBuffer>& ib);

private:
	std::vector<std::unique_ptr<IndexBuffer>> m_index_buffers;
};