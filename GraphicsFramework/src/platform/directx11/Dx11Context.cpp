#include "stdafx.h"

#include "Dx11Context.h"
#include "Dx11HelperMethods.h"
#include "graphics/Window.h"
#include "platform/directx11/Dx11Renderer.h"

#include "Dx11ShaderProgram.h"
#include "Dx11VertexShader.h"
#include "Dx11FragmentShader.h"

#include "Dx11IndexBuffer.h"
#include "Dx11VertexLayout.h"
#include "Dx11VertexArray.h"
#include "Dx11VertexBuffer.h"

#include "graphics/Window.h"

#include "Dx11CommandQueue.h"
#include "Dx11CommandList.h"
#include "Dx11Swapchain.h"

Dx11Context::Dx11Context(Window* window):
	m_window(window)
{
	spdlog::info("Using DirectX 11");

	m_is_vsync = window->GetPropeties().vsync;
	//this->InitD3D(window);
}

Dx11Context::~Dx11Context()
{

}

void Dx11Context::PreInitialize()
{
	this->InitD3D(m_window);
}

void Dx11Context::Initialize()
{
	m_renderer = std::make_unique<Dx11Renderer>(m_command_queue->As<Dx11CommandQueue>()->GetDeviceContext(), m_render_target_view.Get(), m_depth_stencil_view.Get());
}

void Dx11Context::InitD3D(Window* window)
{
	HWND handle = (HWND)window->GetHandle();
	Window::Properties properties = window->GetPropeties();

	m_command_queue = std::make_unique<Dx11CommandQueue>(MAX_NR_OF_FRAMES_IN_FLIGHT);
	m_command_list = m_command_queue->As<Dx11CommandQueue>()->GetCommandList();
	m_swapchain = std::make_unique<Dx11Swapchain>(m_back_buffer_format, (HWND)m_window->GetHandle(), m_window->GetPropeties().is_fullscreen);

	// Swapchain can't use window's width and height directly cause they'll scale incorectly.
	// instead get swapchain's desc after creation and use these width and height values.
	DXGI_SWAP_CHAIN_DESC swapchain_desc;
	m_swapchain->GetSwapchain()->GetDesc(&swapchain_desc);

	this->ResizeBuffers(swapchain_desc.BufferDesc.Width, swapchain_desc.BufferDesc.Height);
}

void Dx11Context::ResizeBuffers(unsigned int width, unsigned int height)
{
	DXCALL(m_swapchain->GetSwapchain()->ResizeBuffers(2, width, height, m_back_buffer_format.ToDirectX(), 0));

	ComPtr<ID3D11Texture2D> back_buffer;
	DXCALL(m_swapchain->GetSwapchain()->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf())));
	DXCALL(m_command_queue->As<Dx11CommandQueue>()->GetDevice()->CreateRenderTargetView(back_buffer.Get(), NULL, m_render_target_view.ReleaseAndGetAddressOf()));
	
	D3D11_TEXTURE2D_DESC depth_stencil_desc;
	depth_stencil_desc.Width = width;
	depth_stencil_desc.Height = height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = m_depth_stencil_format.ToDirectX();

	depth_stencil_desc.SampleDesc.Count = 1;	// no msaa
	depth_stencil_desc.SampleDesc.Quality = 0;  // no msaa
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	DXCALL(m_command_queue->As<Dx11CommandQueue>()->GetDevice()->CreateTexture2D(&depth_stencil_desc, 0, m_depth_stencil_buffer.ReleaseAndGetAddressOf()));
	DXCALL(m_command_queue->As<Dx11CommandQueue>()->GetDevice()->CreateDepthStencilView(m_depth_stencil_buffer.Get(), 0, m_depth_stencil_view.ReleaseAndGetAddressOf()));
	m_command_list->As<Dx11CommandList>()->SetRenderTarget(m_render_target_view.Get(), m_depth_stencil_view.Get());

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (float)width;
	m_viewport.Height = (float)height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_command_queue->As<Dx11CommandQueue>()->GetDeviceContext()->RSSetViewports(1, &m_viewport);

	ComPtr<ID3D11RasterizerState> rasterizer_state;
	DXCALL(m_command_queue->As<Dx11CommandQueue>()->GetDevice()->CreateRasterizerState(&m_rasterizer_settings.ToDirectX11(), rasterizer_state.GetAddressOf()));
	m_command_queue->As<Dx11CommandQueue>()->GetDeviceContext()->RSSetState(rasterizer_state.Get());
}

void Dx11Context::Begin()
{
	m_command_list = m_command_queue->As<Dx11CommandQueue>()->GetCommandList();
	m_command_list->As<Dx11CommandList>()->SetRenderTarget(m_render_target_view.Get(), m_depth_stencil_view.Get());
}

void Dx11Context::Present()
{
	DXCALL(m_swapchain->GetSwapchain()->Present(m_is_vsync, 0));
}

API Dx11Context::GetApiType() const
{
	return API::DIRECTX11;
}

ID3D11Device* Dx11Context::GetDevice() const
{
	return m_command_queue->As<Dx11CommandQueue>()->GetDevice();
}

Dx11CommandList* Dx11Context::GetCommandList() const
{
	return m_command_list->As<Dx11CommandList>();
}

const Dx11VertexShader* Dx11Context::GetBoundVertexShader() const
{
	if (m_bound_shader_program)
		return m_bound_shader_program->GetVertexShader()->As<Dx11VertexShader>();

	return nullptr;
}

void Dx11Context::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	const Dx11IndexBuffer* dx_ib = indexBuffer->As<Dx11IndexBuffer>();
	const unsigned int offset = 0;

	m_command_list->As<Dx11CommandList>()->SetPrimitiveTopology(dx_ib->GetTopology().ToDirectX());
	m_command_list->As<Dx11CommandList>()->SetIndexBuffer(dx_ib->GetBuffer(), dx_ib->GetFormat().ToDirectX(), offset);
}

void Dx11Context::UnbindIndexBufferInternal(const IndexBuffer * indexBuffer)
{
	m_command_list->As<Dx11CommandList>()->SetPrimitiveTopology(Topology(ETopology::UNDEFINED).ToDirectX());
	m_command_list->As<Dx11CommandList>()->SetIndexBuffer(nullptr, Format(EFormat::UNKNOWN).ToDirectX(), 0);
}

void Dx11Context::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	// Currently can only bind 1 vertex buffer at a time

	const Dx11VertexLayout* dx_layout = vertexArray->GetVertexLayout()->As<Dx11VertexLayout>();
	const Dx11VertexBuffer* dx_vb = vertexArray->GetVertexBuffer()->As<Dx11VertexBuffer>();

	unsigned int offset = 0;
	unsigned int stride = dx_layout->GetSize();

	ID3D11Buffer* buffer = dx_vb->GetBuffer();

	unsigned int input_slot = 0;

	m_command_list->As<Dx11CommandList>()->SetVertexBuffer(buffer, stride);
	m_command_list->As<Dx11CommandList>()->SetInputLayout(dx_layout->GetLayout());
}

void Dx11Context::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
	m_command_list->As<Dx11CommandList>()->SetVertexBuffer(nullptr, 0);
	m_command_list->As<Dx11CommandList>()->SetInputLayout(nullptr);
}

void Dx11Context::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	const Dx11VertexShader* dx_vs = shaderProgram->GetVertexShader()->As<Dx11VertexShader>();
	const Dx11FragmentShader* dx_fs = shaderProgram->GetFragmentShader()->As<Dx11FragmentShader>();

	m_command_list->As<Dx11CommandList>()->SetVertexShader(dx_vs->GetShader());
	m_command_list->As<Dx11CommandList>()->SetPixelShader(dx_fs->GetShader());
}

void Dx11Context::UnbindShaderProgramInternal(const ShaderProgram * shaderProgram)
{
	m_command_list->As<Dx11CommandList>()->SetVertexShader(nullptr);
	m_command_list->As<Dx11CommandList>()->SetPixelShader(nullptr);
}

