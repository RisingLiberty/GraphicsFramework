#pragma once

#include "graphics/DownloadBuffer.h"

class Dx12DownloadBuffer : public DownloadBuffer
{
public: 
	Dx12DownloadBuffer();
	virtual ~Dx12DownloadBuffer();

	void Download(const VertexBuffer* vb);

private:
	ComPtr<ID3D12Resource> m_buffer;
};