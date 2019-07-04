#pragma once

#include "graphics/VertexBuffer.h"

class Dx12VertexBuffer : public VertexBuffer
{
public:
	Dx12VertexBuffer(size_t size, BufferUsage usage, const void* data = nullptr);
	virtual ~Dx12VertexBuffer();

	void SetData(const void* data);
	
	virtual void Bind() const override;
	virtual void Unbind() const override;

	ID3D12Resource* GetBufferGpu() const;

private:
	ComPtr<ID3DBlob> m_buffer_cpu;
	ComPtr<ID3D12Resource> m_buffer_gpu;
	ComPtr<ID3D12Resource> m_upload_buffer;
};