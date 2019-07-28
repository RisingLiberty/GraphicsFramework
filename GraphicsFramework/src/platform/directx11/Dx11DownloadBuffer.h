#pragma once

#include "graphics/DownloadBuffer.h"

class Dx11DownloadBuffer : public DownloadBuffer
{
public:
	Dx11DownloadBuffer();
	virtual ~Dx11DownloadBuffer();

	void Download(const VertexBuffer* vb) override;

private:
	ComPtr<ID3D11Buffer> m_buffer;
};