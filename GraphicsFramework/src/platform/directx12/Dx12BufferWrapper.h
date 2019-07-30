#pragma once

#include "graphics/ApiBufferWrapper.h"

class Dx12BufferWrapper : public ApiBufferWrapper
{
public:
	Dx12BufferWrapper(unsigned int size, BufferUsage usage, const void* data);
	virtual ~Dx12BufferWrapper();

	ID3D12Resource* GetBufferGpu() const;

protected:
	void SetDataInternal(const void* data, unsigned int size) override;
	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer(unsigned int size) const override;

private:
	ComPtr<ID3DBlob> m_buffer_cpu;
	ComPtr<ID3D12Resource> m_buffer_gpu;
	ComPtr<ID3D12Resource> m_upload_buffer;
};