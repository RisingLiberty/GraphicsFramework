#include "stdafx.h"

#include "Dx12Swapchain.h"
#include "Dx12HelperMethods.h"
#include "Dx12Resource.h"

#include "commands/Dx12SwitchResourceStateCommand.h"

#include "graphics/Window.h"

Dx12Swapchain::Dx12Swapchain(Window* window, Format format, unsigned int bufferCount, bool isVSync):
	m_buffer_count(bufferCount),
	m_is_v_sync(isVSync)
{
	DXGI_SWAP_CHAIN_DESC desc;
	desc.BufferDesc.Width = 0;// window->GetPropeties().width;
	desc.BufferDesc.Height = 0;// window->GetPropeties().height;
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

	DXCALL(GetDxgiFactory()->CreateSwapChain(GetDx12CommandQueue()->GetApiQueue(), &desc, m_swapchain.GetAddressOf()));

	m_swapchain_buffers.resize(m_buffer_count);

    for (UINT i = 0; i < m_buffer_count; i++)
    {
        ComPtr<ID3D12Resource> buffer;
		DXCALL(m_swapchain->GetBuffer(i, IID_PPV_ARGS(buffer.GetAddressOf())));
        m_swapchain_buffers[i] = std::make_unique<Dx12Resource>(buffer, D3D12_RESOURCE_STATE_PRESENT);
        std::wstring buffer_name = L"Swapchain buffer" + i;
        m_swapchain_buffers[i]->GetApiResource()->SetName(buffer_name.c_str());
    }

    m_swapchain_buffers.front()->ForceSetResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET);
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
	for (unsigned int i = 0; i < m_buffer_count; ++i)
		m_swapchain_buffers[i].reset();

	DXCALL(m_swapchain->ResizeBuffers(m_buffer_count, width, height, format.ToDirectX(), flags));

    for (UINT i = 0; i < m_buffer_count; i++)
    {
        ID3D12Resource* buffer;
		DXCALL(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer)));
        m_swapchain_buffers[i] = std::make_unique<Dx12Resource>(buffer, D3D12_RESOURCE_STATE_COMMON);
    }

}

IDXGISwapChain* Dx12Swapchain::GetSwapchain() const
{
	return m_swapchain.Get();
}

Dx12Resource* Dx12Swapchain::GetBuffer(unsigned int index) const
{
	return m_swapchain_buffers[index].get();
}

DXGI_SWAP_CHAIN_DESC Dx12Swapchain::GetDesc() const
{
	DXGI_SWAP_CHAIN_DESC desc;
	m_swapchain->GetDesc(&desc);
	return desc;
}