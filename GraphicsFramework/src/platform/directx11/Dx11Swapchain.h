#pragma once

#include "graphics/BufferHelpers.h"

class Dx11Swapchain
{
public:
	Dx11Swapchain(Format format, HWND hwnd, bool isFullScreen);
	~Dx11Swapchain();

	IDXGISwapChain* GetSwapchain() const;

private:
	ComPtr<IDXGISwapChain> m_swapchain;
};