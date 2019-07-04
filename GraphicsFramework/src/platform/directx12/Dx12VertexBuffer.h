#pragma once

#include "graphics/VertexBuffer.h"

class Dx12VertexBuffer : public VertexBuffer
{
public:
	Dx12VertexBuffer(size_t size, BufferUsage usage, void* data = nullptr);
	virtual ~Dx12VertexBuffer();

	ID3D12Resource* GetBuffer() const;

private:
	ComPtr<ID3D12Resource> m_buffer;
};