#pragma once

#include "graphics/VertexBuffer.h"
#include "Dx12BufferWrapper.h"

class Dx12VertexBuffer : public VertexBuffer, public Dx12BufferWrapper
{
public:
	Dx12VertexBuffer(unsigned int size, BufferUsage usage, const void* data = nullptr);
	virtual ~Dx12VertexBuffer();

	void SetData(const void* data);
	std::unique_ptr<byte> GetData() const;
};