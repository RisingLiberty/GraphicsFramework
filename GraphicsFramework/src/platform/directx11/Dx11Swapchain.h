#pragma once

#include "graphics/BufferHelpers.h"

class Dx11Swapchain
{
public:
	Dx11Swapchain(Format format, HWND hwnd, bool isFullScreen);
	~Dx11Swapchain();

	IDXGISwapChain* GetSwapchain() const;

	DXGI_SWAP_CHAIN_DESC GetDesc() const;

private:
	ComPtr<IDXGISwapChain> m_swapchain;
};