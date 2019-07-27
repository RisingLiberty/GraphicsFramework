#pragma once

#include "graphics/IndexBuffer.h"

class Dx11IndexBuffer : public IndexBuffer
{
public:
	Dx11IndexBuffer(size_t count, Format format, Topology topology, BufferUsage usage, void* data);
	virtual ~Dx11IndexBuffer();

	virtual void SetData(const void* data) override;

	ID3D11Buffer* GetBuffer() const;

private:
	ComPtr<ID3D11Buffer> m_buffer;
};

