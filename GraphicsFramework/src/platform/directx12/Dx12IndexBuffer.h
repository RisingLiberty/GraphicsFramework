#pragma once

#include "graphics/IndexBuffer.h"

class Dx12IndexBuffer : public IndexBuffer
{
public:
	Dx12IndexBuffer(size_t count, Format format, Topology topology, BufferUsage usage, const void* data = nullptr);
	virtual ~Dx12IndexBuffer();

	virtual void SetData(const void* data) override;

	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

private:
	ComPtr<ID3DBlob> m_buffer_cpu;
	ComPtr<ID3D12Resource> m_buffer_gpu;
	ComPtr<ID3D12Resource> m_upload_buffer;
};