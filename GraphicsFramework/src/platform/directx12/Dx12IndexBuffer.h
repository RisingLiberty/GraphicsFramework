#pragma once

#include "graphics/IndexBuffer.h"
#include "Dx12BufferWrapper.h"

class Dx12IndexBuffer : public IndexBuffer, public Dx12BufferWrapper
{
public:
	Dx12IndexBuffer(unsigned int  count, Format format, Topology topology, BufferUsage usage, BufferAccess access, const void* data = nullptr);
	virtual ~Dx12IndexBuffer();

	std::unique_ptr<byte> GetData() const override;
	void SetData(const void* data) override;

	D3D12_INDEX_BUFFER_VIEW GetBufferView() const;
};