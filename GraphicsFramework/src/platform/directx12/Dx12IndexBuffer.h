#pragma once

#include "graphics/IndexBuffer.h"

class Dx12IndexBuffer : public IndexBuffer
{
public:
	Dx12IndexBuffer(size_t count, BufferUsage usage, const void* data = nullptr);
	virtual ~Dx12IndexBuffer();

	virtual void SetData(const void* data) override;
	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
	ComPtr<ID3DBlob> m_buffer_cpu;
	ComPtr<ID3D12Resource> m_buffer_gpu;
	ComPtr<ID3D12Resource> m_upload_buffer;
};