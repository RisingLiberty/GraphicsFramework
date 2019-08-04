#pragma once

class Window;

#include "graphics/BufferHelpers.h"

class Dx12Swapchain
{
public:
	Dx12Swapchain(Window* window, Format format, unsigned int bufferCount, bool isVSync = true);
	~Dx12Swapchain();

	void Present();

	void ResizeBuffers(unsigned int width, unsigned int height, Format format, DXGI_SWAP_CHAIN_FLAG flags);

	IDXGISwapChain* GetSwapchain() const;
	ID3D12Resource* GetBuffer(unsigned int index) const;

private:
	std::vector<ComPtr<ID3D12Resource>> m_swapchain_buffers;
	ComPtr<IDXGISwapChain> m_swapchain;
	unsigned int m_buffer_count;
	bool m_is_v_sync;
};