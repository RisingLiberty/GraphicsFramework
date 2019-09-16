#include "stdafx.h"

#include "Dx11Swapchain.h"
#include "Dx11HelperMethods.h"

Dx11Swapchain::Dx11Swapchain(Format format, HWND hwnd, bool isFullScreen)
{
	DXGI_SWAP_CHAIN_DESC swapchain_desc;
	ZeroMemory(&swapchain_desc, sizeof(swapchain_desc));

	// specifying 0 makes the desc use the window's renderable rectangle
	swapchain_desc.BufferDesc.Width = 0;
	swapchain_desc.BufferDesc.Height = 0;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferDesc.Format = format.ToDirectX();

	swapchain_desc.SampleDesc.Count = 1;	// no msaa
	swapchain_desc.SampleDesc.Quality = 0;  // no msaa

	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = 2;
	swapchain_desc.OutputWindow = hwnd;
	swapchain_desc.Windowed = !isFullScreen;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGIDevice> dxgi_device = nullptr;
	ComPtr<IDXGIAdapter> dxgi_adapter = nullptr;
	ComPtr<IDXGIFactory> dxgi_factory = nullptr;

	DXCALL(GetDx11Device()->QueryInterface(IID_PPV_ARGS(dxgi_device.GetAddressOf()));
	DXCALL(dxgi_device->GetParent(IID_PPV_ARGS(dxgi_adapter.GetAddressOf())));
	DXCALL(dxgi_adapter->GetParent(IID_PPV_ARGS(dxgi_factory.GetAddressOf())));
	DXCALL(dxgi_factory->CreateSwapChain(GetDx11Device(), &swapchain_desc, m_swapchain.ReleaseAndGetAddressOf())));

}

Dx11Swapchain::~Dx11Swapchain()
{

}

DXGI_SWAP_CHAIN_DESC Dx11Swapchain::GetDesc() const
{
	DXGI_SWAP_CHAIN_DESC desc;
	m_swapchain->GetDesc(&desc);
	return desc;
}

IDXGISwapChain* Dx11Swapchain::GetSwapchain() const
{
	return m_swapchain.Get();
}