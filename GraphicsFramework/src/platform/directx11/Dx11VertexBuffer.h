#pragma once

#include "graphics/VertexBuffer.h"
#include "Dx11BufferWrapper.h"

class Dx11VertexBuffer : public VertexBuffer, public Dx11BufferWrapper
{
public:
	Dx11VertexBuffer(unsigned int size, BufferUsage usage, void* data);
	virtual ~Dx11VertexBuffer();

	void SetData(const void* data) override;
	std::unique_ptr<byte> GetData() const override;

};

