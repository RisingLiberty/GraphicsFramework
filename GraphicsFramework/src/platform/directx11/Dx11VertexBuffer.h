#pragma once

#include "graphics/VertexBuffer.h"

class Dx11VertexBuffer : public VertexBuffer
{
public:
	Dx11VertexBuffer(size_t size, BufferUsage usage, void* data);
	virtual ~Dx11VertexBuffer();

	virtual void SetData(void* data) override;
	virtual void Bind() const override;
	virtual void Unbind() const override;

	ID3D11Buffer* GetBuffer() const;

private:
	ComPtr<ID3D11Buffer> m_buffer;
};

