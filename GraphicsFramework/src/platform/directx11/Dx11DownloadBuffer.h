#pragma once

#include "graphics/DownloadBuffer.h"

class Dx11DownloadBuffer : public DownloadBuffer
{
public:
	Dx11DownloadBuffer(unsigned int size);
	virtual ~Dx11DownloadBuffer();

	void Download(const ApiBufferWrapper* buffer) override;

private:
	ComPtr<ID3D11Buffer> m_buffer;
};