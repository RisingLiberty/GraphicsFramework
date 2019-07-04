#include "stdafx.h"

#include "Dx12VertexBuffer.h"

Dx12VertexBuffer::Dx12VertexBuffer(size_t size, BufferUsage usage, void* data):
	VertexBuffer(size, usage)
{

}

Dx12VertexBuffer::~Dx12VertexBuffer()
{

}

ID3D12Resource* Dx12VertexBuffer::GetBuffer() const
{
	return m_buffer.Get();
}
