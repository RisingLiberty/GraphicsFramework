#pragma once

#include "graphics/IndexBuffer.h"

class Dx12IndexBuffer : public IndexBuffer
{
public:
	Dx12IndexBuffer(size_t count, BufferUsage usage, void* data = nullptr);
	~Dx12IndexBuffer();

	void Bind() const override;
	void Unbind() const override;

private:
	ComPtr<ID3D12Resource> m_buffer;
};