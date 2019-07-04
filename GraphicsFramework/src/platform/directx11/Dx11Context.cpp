#include "stdafx.h"

#include "Dx11Context.h"
#include "Dx11HelperMethods.h"
#include "platform/win64/Win64Window.h"
#include "platform/directx11/Dx11Renderer.h"
#include "platform/directx11/dx11VertexShader.h"
#include "platform/directx11/dx11FragmentShader.h"

Dx11Context::Dx11Context(Window* window)
{
	spdlog::info("Using DirectX 11");

	m_is_vsync = window->GetPropeties().vsync;
	this->InitD3D(window);
	m_renderer = std::make_unique<Dx11Renderer>(m_render_target_view.Get(), m_depth_stencil_view.Get());

}

Dx11Context::~Dx11Context()
{

}

void Dx11Context::InitD3D(Window* window)
{
	Win64Window* win64_window = dynamic_cast<Win64Window*>(window);
	assert(win64_window);

	HWND handle = (HWND)win64_window->GetHandle();
	Window::Properties properties = win64_window->GetPropeties();

	UINT creationFlags = 0; //D3D11_CREATE_DEVICE_BGRA_SUPPORT <-- not needed, but good to have on stand by
#if defined(_DEBUG)
	// Let's DirectX log to visual studio's output tab.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXCALL(D3D11CreateDevice(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		creationFlags,
		NULL, 
		NULL, 
		D3D11_SDK_VERSION, 
		m_resources.device.ReleaseAndGetAddressOf(), 
		&m_feature_level, 
		m_resources.device_context.ReleaseAndGetAddressOf()
	));

	DXGI_SWAP_CHAIN_DESC swapchain_desc;
	ZeroMemory(&swapchain_desc, sizeof(swapchain_desc));

	swapchain_desc.BufferDesc.Width = properties.width;
	swapchain_desc.BufferDesc.Height = properties.height;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapchain_desc.SampleDesc.Count = 1;	// no msaa
	swapchain_desc.SampleDesc.Quality = 0;  // no msaa

	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = 2;
	swapchain_desc.OutputWindow = handle;
	swapchain_desc.Windowed = !properties.is_fullscreen;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGIDevice> dxgi_device = nullptr;
	ComPtr<IDXGIAdapter> dxgi_adapter = nullptr;
	ComPtr<IDXGIFactory> dxgi_factory = nullptr;

	DXCALL(m_resources.device->QueryInterface(IID_PPV_ARGS(dxgi_device.GetAddressOf()));
	DXCALL(dxgi_device->GetParent(IID_PPV_ARGS(dxgi_adapter.GetAddressOf())));
	DXCALL(dxgi_adapter->GetParent(IID_PPV_ARGS(dxgi_factory.GetAddressOf())));
	DXCALL(dxgi_factory->CreateSwapChain(m_resources.device.Get(), &swapchain_desc, m_resources.swapchain.ReleaseAndGetAddressOf())));

#if defined _DEBUG
	DXCALL(m_resources.device->QueryInterface(IID_PPV_ARGS(m_debug_layer.ReleaseAndGetAddressOf())));
	DXCALL(m_debug_layer->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL));

	//TODO: Look up further information about this
	ID3D11InfoQueue* info_queue;
	DXCALL(m_resources.device->QueryInterface(IID_PPV_ARGS(&info_queue)));
	D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
	D3D11_INFO_QUEUE_FILTER filter;
	ZeroMemory(&filter, sizeof(filter));
	filter.DenyList.NumIDs = 1;
	filter.DenyList.pIDList = hide;
	DXCALL(info_queue->AddStorageFilterEntries(&filter));
#endif

	this->ResizeBuffers(properties.width, properties.height);
}

void Dx11Context::ResizeBuffers(unsigned int width, unsigned int height)
{
	DXCALL(m_resources.swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ComPtr<ID3D11Texture2D> back_buffer;
	DXCALL(m_resources.swapchain->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf())));
	DXCALL(m_resources.device->CreateRenderTargetView(back_buffer.Get(), NULL, m_render_target_view.ReleaseAndGetAddressOf()));
	
	D3D11_TEXTURE2D_DESC depth_stencil_desc;
	depth_stencil_desc.Width = width;
	depth_stencil_desc.Height = height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depth_stencil_desc.SampleDesc.Count = 1;	// no msaa
	depth_stencil_desc.SampleDesc.Quality = 0;  // no msaa
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	DXCALL(m_resources.device->CreateTexture2D(&depth_stencil_desc, 0, m_depth_stencil_buffer.ReleaseAndGetAddressOf()));
	DXCALL(m_resources.device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), 0, m_depth_stencil_view.ReleaseAndGetAddressOf()));
	SetRenderTargets(m_render_target_view.Get(), m_depth_stencil_view.Get());

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (float)width;
	m_viewport.Height = (float)height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_resources.device_context->RSSetViewports(1, &m_viewport);

	D3D11_RASTERIZER_DESC rasterizer_desc;
	rasterizer_desc.AntialiasedLineEnable = false;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0.0f;
	rasterizer_desc.DepthClipEnable = true;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.FrontCounterClockwise = true;
	rasterizer_desc.MultisampleEnable = false;
	rasterizer_desc.ScissorEnable = false;
	rasterizer_desc.SlopeScaledDepthBias = 0.0f;

	ComPtr<ID3D11RasterizerState> rasterizer_state;
	DXCALL(m_resources.device->CreateRasterizerState(&rasterizer_desc, rasterizer_state.GetAddressOf()));
	m_resources.device_context->RSSetState(rasterizer_state.Get());
}

void Dx11Context::Present()
{
	DXCALL(m_resources.swapchain->Present(m_is_vsync, 0));
}

Context::API Dx11Context::GetApiType() const
{
	return API::DIRECTX11;
}

Dx11Context::Dx11Resources Dx11Context::GetResources() const
{
	return m_resources;
}

ID3D11Device* Dx11Context::GetDevice() const
{
	return m_resources.device.Get();
}

ID3D11DeviceContext* Dx11Context::GetDeviceContext() const
{
	return m_resources.device_context.Get();
}

void Dx11Context::BindVertexShader(Dx11VertexShader* vertexShader)
{
	m_resources.device_context->VSSetShader(vertexShader->GetShader(), NULL, 0);
	m_bound_vertex_shader = vertexShader;
}

void Dx11Context::BindFragmentShader(Dx11FragmentShader* fragmentShader)
{
	m_resources.device_context->PSSetShader(fragmentShader->GetShader(), NULL, 0);
	m_bound_fragment_shader = fragmentShader;
}

Dx11VertexShader* Dx11Context::GetBoundVertexShader()
{
	return m_bound_vertex_shader;
}

Dx11FragmentShader* Dx11Context::GetBoundFragmentShader()
{
	return m_bound_fragment_shader;
}

void Dx11Context::SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view)
{
	m_resources.device_context->OMSetRenderTargets(1, &target, view);
}
