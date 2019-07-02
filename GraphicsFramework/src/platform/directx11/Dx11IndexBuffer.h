#pragma once

#include "graphics/IndexBuffer.h"

class Dx11IndexBuffer : public IndexBuffer
{
public:
	Dx11IndexBuffer(size_t count, BufferUsage usage, void* data);
	virtual ~Dx11IndexBuffer();

	virtual void SetData(void* data) override;
	virtual void Bind() const override;
	virtual void Unbind() const override;

	ID3D11Buffer* GetBuffer() const;

private:
	ComPtr<ID3D11Buffer> m_buffer;
};

