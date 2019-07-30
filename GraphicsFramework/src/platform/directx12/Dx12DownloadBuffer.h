#pragma once

#include "graphics/DownloadBuffer.h"

class Dx12DownloadBuffer : public DownloadBuffer
{
public: 
	Dx12DownloadBuffer(unsigned int size);
	virtual ~Dx12DownloadBuffer();

	void Download(const ApiBufferWrapper* buffer) override;

private:
	ComPtr<ID3D12Resource> m_buffer;
};