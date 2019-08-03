#include "stdafx.h"
#include "Dx12Context.h"
#include "Dx12HelperMethods.h"

#include "Dx12IndexBuffer.h"
#include "Dx12VertexShader.h"
#include "Dx12VertexLayout.h"
#include "Dx12FragmentShader.h"
#include "Dx12VertexArray.h"
#include "Dx12ShaderProgram.h"
#include "Dx12UploadBuffer.h"

#if defined _DEBUG
//#define LOG_ADAPATERS
#endif

#include "graphics/Window.h"
#include "Dx12Renderer.h"

#include "Dx12DepthStencilViewDescriptorHeap.h"
#include "Dx12RenderTargetViewDescriptorHeap.h"
#include "Dx12CbvSrvUavDescriptorHeap.h"

#include "Dx12CommandQueue.h"
#include "Dx12CommandList.h"

Dx12Context::Dx12Context(Window* window):
	m_window(window)
{
	spdlog::info("Using DirectX 12");

	this->InitD3D(window);
	m_is_vsync = window->GetPropeties().vsync;
}

Dx12Context::~Dx12Context() = default;

void Dx12Context::PreInitialize()
{
	CreateCommandObjects();
	CreateSwapChain(m_window);

	m_rtv_heap = std::make_unique<Dx12RenderTargetViewDescriptorHeap>(SWAPCHAIN_BUFFER_COUNT);
	m_dsv_heap = std::make_unique<Dx12DepthStencilViewDescriptorHeap>(1);

	this->OnResize(m_window->GetPropeties().width, m_window->GetPropeties().height);

	// Reset the command list to prep for initialization commands.
	m_command_list->Reset();
}

void Dx12Context::Initialize()
{
	m_cbv_heap = std::make_unique<Dx12CbvSrvUavDescriptorHeap>(1, true);
	m_cbv_imgui_heap = std::make_unique<Dx12CbvSrvUavDescriptorHeap>(1, true);

	m_renderer = std::make_unique<Dx12Renderer>();
}

void Dx12Context::PostInitialize()
{
	// Execute the initialization commands.
	m_command_queue->Execute();

	// Wait until initialization is complete.
	m_command_queue->Flush();
}

void Dx12Context::InitD3D(Window* window)
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	{
		ComPtr<ID3D12Debug> debugController;
		DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif

	DXCALL(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgi_factory)));

	// Try to create hardware device.
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_device));

	// Fallback to WARP device.
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		DXCALL(m_dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

		DXCALL(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)));
	}

	m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cb_srv_uav_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = m_back_buffer_format.ToDirectX();
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	DXCALL(m_device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	assert(msQualityLevels.NumQualityLevels > 0 && "Unexpected MSAA quality level.");

#if defined LOG_ADAPATERS
	LogAdapters();
#endif

}

void Dx12Context::OnResize(unsigned int width, unsigned int height)
{
	assert(m_device);
	assert(m_swapchain);

	m_command_list->Reset();

	// Release the previous resources we will be recreating.
	for (int i = 0; i < SWAPCHAIN_BUFFER_COUNT; ++i)
		m_swapchain_buffers[i].Reset();
	m_depth_stencil_buffer.Reset();

	// Resize the swap chain.
	DXCALL(m_swapchain->ResizeBuffers(
		SWAPCHAIN_BUFFER_COUNT,
		width, height,
		m_back_buffer_format.ToDirectX(),
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	m_current_back_buffer_index = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtv_heap->GetCpuView());
	for (UINT i = 0; i < SWAPCHAIN_BUFFER_COUNT; i++)
	{
		DXCALL(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_swapchain_buffers[i])));
		m_device->CreateRenderTargetView(m_swapchain_buffers[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtv_descriptor_size);
	}

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
	// the depth buffer.  Therefore, because we need to create two views to the same resource:
	//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
	// we need to create the depth buffer resource with a typeless format.  
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = m_depth_stencil_format.ToDirectX();
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	DXCALL(m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optClear,
		IID_PPV_ARGS(m_depth_stencil_buffer.GetAddressOf())));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = m_depth_stencil_format.ToDirectX();
	dsvDesc.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), &dsvDesc, this->GetDepthStencilView());

	// Execute the resize commands.
	m_command_queue->Execute();

	// Update the viewport transform to cover the client area.
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_scissor_rect = { 0, 0, (LONG)width, (LONG)height};
}

void Dx12Context::CreateCommandObjects()
{
	m_command_queue = std::make_unique<Dx12CommandQueue>();
	m_command_list = m_command_queue->GetApiCommandList();
	m_command_list->Close();
}

void Dx12Context::CreateSwapChain(Window* window)
{
	// Release the previous swapchain we will be recreating.
	m_swapchain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = window->GetPropeties().width;
	sd.BufferDesc.Height = window->GetPropeties().height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_back_buffer_format.ToDirectX();
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SWAPCHAIN_BUFFER_COUNT;
	sd.OutputWindow = (HWND)window->GetHandle();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	DXCALL(m_dxgi_factory->CreateSwapChain(
		m_command_queue->GetApiQueue(),
		&sd,
		m_swapchain.GetAddressOf()));
}

void Dx12Context::CreateRtvDescriptorHeaps()
{
	m_rtv_heap = std::make_unique<Dx12RenderTargetViewDescriptorHeap>(SWAPCHAIN_BUFFER_COUNT);
}

void Dx12Context::CreateDsvDescriptorHeap()
{
	m_dsv_heap = std::make_unique<Dx12DepthStencilViewDescriptorHeap>(1);
}

void Dx12Context::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<ComPtr<IDXGIAdapter>> adapterList;
	while (m_dxgi_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;

		std::string string_8bit(text.begin(), text.end());
		spdlog::info(string_8bit);

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
		LogAdapterOutputs(adapterList[i].Get());
}

void Dx12Context::LogAdapterOutputs(IDXGIAdapter * adapter)
{
	UINT i = 0;
	ComPtr<IDXGIOutput> output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;

		std::string string_8bit(text.begin(), text.end());
		spdlog::info(string_8bit);

		LogOutputDisplayModes(output.Get(), m_back_buffer_format.ToDirectX());

		++i;
	}
}

void Dx12Context::LogOutputDisplayModes(IDXGIOutput * output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring((float)n/(float)d);

		std::string string_8bit(text.begin(), text.end());
		spdlog::info(string_8bit);
	}
}

void Dx12Context::BuildPSO()
{
	if (!m_bound_shader_program || m_pipeline_state)
		return;

	auto dx_vs = static_cast<Dx12VertexShader*>(m_bound_shader_program->GetVertexShader());
	auto dx_fs = static_cast<Dx12FragmentShader*>(m_bound_shader_program->GetFragmentShader());

	const Dx12VertexLayout* dx_vertex_layout = static_cast<const Dx12VertexLayout*>(m_bound_vertex_array->GetVertexLayout());
	const Dx12ShaderProgram* dx_shader_program = static_cast<const Dx12ShaderProgram*>(m_bound_shader_program);

	Dx12UploadBuffer* upload_buffer = dx_shader_program->GetUploadBuffer();
	
	D3D12_GPU_VIRTUAL_ADDRESS cb_address = upload_buffer->GetResource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc;
	cbv_desc.BufferLocation = cb_address;
	cbv_desc.SizeInBytes = upload_buffer->GetSize();

	m_device->CreateConstantBufferView(&cbv_desc, m_cbv_heap->GetCpuView());

	float color[] = { 1.0f, 0.3f, 0.8f, 1.0f };
	m_bound_shader_program->SetUniform("u_Color", color, sizeof(float) * 4);
	m_bound_shader_program->UploadVariables();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { dx_vertex_layout->GetLayout().data(), (UINT)dx_vertex_layout->GetLayout().size() };
	psoDesc.pRootSignature = dx_shader_program->GetRootSignature();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(dx_vs->GetCompiledCode()->GetBufferPointer()),
		dx_vs->GetCompiledCode()->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(dx_fs->GetCompiledCode()->GetBufferPointer()),
		dx_fs->GetCompiledCode()->GetBufferSize()
	};

	psoDesc.RasterizerState = m_rasterizer_settings.ToDirectX12();
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = m_bound_index_buffer->GetTopology().ToDirectX12();
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = m_back_buffer_format.ToDirectX();
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = m_depth_stencil_format.ToDirectX();
	DXCALL(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(m_pipeline_state.ReleaseAndGetAddressOf())));
}

API Dx12Context::GetApiType() const
{
	return API::DIRECTX12;
}

ID3D12Device* Dx12Context::GetDevice() const
{
	return m_device.Get();
}

ID3D12GraphicsCommandList* Dx12Context::GetCommandList() const
{
	return m_command_queue->GetApiCommandList()->GetApiCommandList();
}

void Dx12Context::BindResourcesToPipeline()
{
	m_command_list->SetDescriptorHeaps(m_cbv_heap->GetHeap());
	m_command_list->SetGraphicsRootSignature(static_cast<const Dx12ShaderProgram*>(m_bound_shader_program)->GetRootSignature());
	m_command_list->SetGraphicsRootDescriptorTable(m_cbv_heap->GetGpuView());
}

void Dx12Context::BindImgui()
{
	m_command_list->SetDescriptorHeaps(m_cbv_imgui_heap->GetHeap());
}

void Dx12Context::Begin()
{
	m_command_list->ResetAlloc();
	m_command_list->Reset();

	if (m_pipeline_state)
		m_command_list->SetPipelineState(m_pipeline_state);
	m_command_list->SwitchResourceToRenderTarget(this->GetCurrentBackBuffer());
	m_command_list->SetViewport(m_viewport);
	m_command_list->SetScissorRects(m_scissor_rect);
	m_command_list->SetRenderTargets(this->GetCurrentBackBufferView(), this->GetDepthStencilView());
	m_command_list->SetDescriptorHeaps(m_cbv_heap->GetHeap());
}

void Dx12Context::Present()
{
	// swap the back and front buffers
	DXCALL(m_swapchain->Present(m_is_vsync, 0));
}

void Dx12Context::End()
{
	// Indicate a state transition on the resource usage.
	m_command_list->SwitchResourceToPresent(this->GetCurrentBackBuffer());

	m_current_back_buffer_index = (m_current_back_buffer_index + 1) % SWAPCHAIN_BUFFER_COUNT;

	m_command_queue->Execute();
	m_command_queue->Flush();
}

unsigned int Dx12Context::GetNrOfFrameResources() const
{
	return NUM_FRAME_RESOURCES;
}

DXGI_FORMAT Dx12Context::GetBackBufferFormat() const
{
	return m_back_buffer_format.ToDirectX();
}

ID3D12DescriptorHeap * Dx12Context::GetSrvDescriptorHeap() const
{
	return m_cbv_imgui_heap->GetHeap();
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Context::GetCurrentBackBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_rtv_heap->GetCpuView(),
		m_current_back_buffer_index,
		m_rtv_descriptor_size);
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Context::GetDepthStencilView() const
{
	return m_dsv_heap->GetCpuView();
}

void Dx12Context::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	D3D12_INDEX_BUFFER_VIEW ib_view = static_cast<const Dx12IndexBuffer*>(m_bound_index_buffer)->GetIndexBufferView();
	m_command_list->SetIndexBuffer(ib_view);
	m_command_list->SetPrimitiveTopology(m_bound_index_buffer->GetTopology().ToDirectX());

	//m_command_list->IASetIndexBuffer(&ib_view);
	//m_command_list->IASetPrimitiveTopology(m_bound_index_buffer->GetTopology().ToDirectX());

	if (m_bound_vertex_array && m_bound_shader_program)
		this->BuildPSO();
}

void Dx12Context::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	//m_command_list->IASetIndexBuffer(nullptr);
	//m_command_list->IASetPrimitiveTopology(Topology(ETopology::UNDEFINED).ToDirectX());
}

void Dx12Context::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	const Dx12VertexArray* dx_va = static_cast<const Dx12VertexArray*>(m_bound_vertex_array);
	D3D12_VERTEX_BUFFER_VIEW vb_view = dx_va->GetVertexBufferView();
	m_command_list->SetVertexBuffer(vb_view);

	//m_command_list->IASetVertexBuffers(0, 1, &vb_view);

	if (m_bound_shader_program && m_bound_index_buffer)
		this->BuildPSO();
}

void Dx12Context::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
	//m_command_list->IASetVertexBuffers(0, 0, nullptr);
}

void Dx12Context::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	if (m_bound_vertex_array && m_bound_index_buffer)
		this->BuildPSO();
}

void Dx12Context::UnbindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
}

ID3D12Resource* Dx12Context::GetCurrentBackBuffer() const
{
	return m_swapchain_buffers[m_current_back_buffer_index].Get();
}