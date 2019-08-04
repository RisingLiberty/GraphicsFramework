#include "stdafx.h"

#include "Dx12Swapchain.h"
#include "Dx12HelperMethods.h"

#include "graphics/Window.h"

Dx12Swapchain::Dx12Swapchain(Window* window, Format format, unsigned int bufferCount, bool isVSync):
	m_buffer_count(bufferCount),
	m_is_v_sync(isVSync)
{
	DXGI_SWAP_CHAIN_DESC desc;
	desc.BufferDesc.Width = window->GetPropeties().width;
	desc.BufferDesc.Height = window->GetPropeties().height;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = format.ToDirectX();
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = bufferCount;

	desc.OutputWindow = (HWND)window->GetHandle();
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXCALL(GetDxgiFactory()->CreateSwapChain(GetDx12CommandQueue(), &desc, m_swapchain.GetAddressOf()));

	m_swapchain_buffers.resize(m_buffer_count);

	for (UINT i = 0; i < m_buffer_count; i++)
		DXCALL(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_swapchain_buffers[i])));
}

Dx12Swapchain::~Dx12Swapchain()
{

}

void Dx12Swapchain::Present()
{
	DXCALL(m_swapchain->Present(m_is_v_sync, 0));
}

void Dx12Swapchain::ResizeBuffers(unsigned int width, unsigned int height, Format format, DXGI_SWAP_CHAIN_FLAG flags)
{
	for (int i = 0; i < m_buffer_count; ++i)
		m_swapchain_buffers[i].Reset();

	DXCALL(m_swapchain->ResizeBuffers(m_buffer_count, width, height, format.ToDirectX(), flags));

	for (UINT i = 0; i < m_buffer_count; i++)
		DXCALL(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_swapchain_buffers[i])));

}

IDXGISwapChain* Dx12Swapchain::GetSwapchain() const
{
	return m_swapchain.Get();
}

ID3D12Resource* Dx12Swapchain::GetBuffer(unsigned int index) const
{
	return m_swapchain_buffers[index].Get();
}