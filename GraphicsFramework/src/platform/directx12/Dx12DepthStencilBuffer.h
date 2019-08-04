#pragma once

#include "graphics/BufferHelpers.h"

class Dx12DepthStencilBuffer
{
public:
	Dx12DepthStencilBuffer(unsigned int width, unsigned int height, Format format);
	~Dx12DepthStencilBuffer();

	ID3D12Resource* GetApiBuffer() const;

private:
	ComPtr<ID3D12Resource> m_buffer;
};