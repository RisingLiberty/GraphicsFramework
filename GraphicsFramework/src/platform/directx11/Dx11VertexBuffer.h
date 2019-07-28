#pragma once

#include "graphics/VertexBuffer.h"

class Dx11VertexBuffer : public VertexBuffer
{
public:
	Dx11VertexBuffer(unsigned int size, BufferUsage usage, void* data);
	virtual ~Dx11VertexBuffer();

	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer() const override;
	void SetData(const void* data) override;

	ID3D11Buffer* GetBuffer() const;

private:
	ComPtr<ID3D11Buffer> m_buffer;
};

