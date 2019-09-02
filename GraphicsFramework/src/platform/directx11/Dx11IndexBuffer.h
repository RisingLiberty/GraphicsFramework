#pragma once

#include "graphics/IndexBuffer.h"
#include "Dx11BufferWrapper.h"

class Dx11IndexBuffer : public IndexBuffer, public Dx11BufferWrapper
{
public:
	Dx11IndexBuffer(unsigned int  count, Format format, Topology topology, BufferUsage usage, BufferAccess access, void* data);
	virtual ~Dx11IndexBuffer();

	std::unique_ptr<byte> GetData() const override;
	void SetData(const void* data) override;
};

