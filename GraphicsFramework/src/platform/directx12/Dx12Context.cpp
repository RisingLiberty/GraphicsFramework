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

Dx12Context::Dx12Context(Window* window)
{
	spdlog::info("Using DirectX 12");

	this->InitD3D(window);
	this->OnResize(window->GetPropeties().width, window->GetPropeties().height);
	m_is_vsync = window->GetPropeties().vsync;
}

Dx12Context::~Dx12Context() = default;

void Dx12Context::PreInitialize()
{
	// Reset the command list to prep for initialization commands.
	DXCALL(m_command_list->Reset(m_direct_cmdlist_alloc.Get(), nullptr));
}

void Dx12Context::Initialize()
{
	BuildDescriptorHeaps();
	m_renderer = std::make_unique<Dx12Renderer>();
}

void Dx12Context::PostInitialize()
{
	// Execute the initialization commands.
	DXCALL(m_command_list->Close());
	ID3D12CommandList* cmdsLists[] = { m_command_list.Get() };
	m_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();
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

	DXCALL(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_fence)));

	m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cb_srv_uav_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = m_back_buffer_format;
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

	CreateCommandObjects();
	CreateSwapChain(window);
	CreateRtvAndDsvDescriptorHeaps();
}

void Dx12Context::OnResize(unsigned int width, unsigned int height)
{
	assert(m_device);
	assert(m_swapchain);
	assert(m_direct_cmdlist_alloc);

	// Flush before changing any resources.
	FlushCommandQueue();

	DXCALL(m_command_list->Reset(m_direct_cmdlist_alloc.Get(), nullptr));

	// Release the previous resources we will be recreating.
	for (int i = 0; i < SWAPCHAIN_BUFFER_COUNT; ++i)
		m_swapchain_buffers[i].Reset();
	m_depth_stencil_buffer.Reset();

	// Resize the swap chain.
	DXCALL(m_swapchain->ResizeBuffers(
		SWAPCHAIN_BUFFER_COUNT,
		width, height,
		m_back_buffer_format,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	m_current_back_buffer_index = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
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
	optClear.Format = m_depth_stencil_format;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	DXCALL(m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(m_depth_stencil_buffer.GetAddressOf())));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = m_depth_stencil_format;
	dsvDesc.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), &dsvDesc, this->GetDepthStencilView());

	// Transition the resource from its initial state to be used as a depth buffer.
	m_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_depth_stencil_buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	// Execute the resize commands.
	DXCALL(m_command_list->Close());
	ID3D12CommandList* cmdsLists[] = { m_command_list.Get() };
	m_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	FlushCommandQueue();

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
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	DXCALL(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_command_queue)));

	DXCALL(m_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(m_direct_cmdlist_alloc.GetAddressOf())));

	DXCALL(m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_direct_cmdlist_alloc.Get(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(m_command_list.GetAddressOf())));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
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
	sd.BufferDesc.Format = m_back_buffer_format;
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
		m_command_queue.Get(),
		&sd,
		m_swapchain.GetAddressOf()));
}

void Dx12Context::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SWAPCHAIN_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	DXCALL(m_device->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(m_rtv_heap.GetAddressOf())));


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	DXCALL(m_device->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(m_dsv_heap.GetAddressOf())));
}

void Dx12Context::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	m_current_fence_value++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	DXCALL(m_command_queue->Signal(m_fence.Get(), m_current_fence_value));

	// Wait until the GPU has completed commands up to this fence point.
	if (m_fence->GetCompletedValue() < m_current_fence_value)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		DXCALL(m_fence->SetEventOnCompletion(m_current_fence_value, eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
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

		LogOutputDisplayModes(output.Get(), m_back_buffer_format);

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

void Dx12Context::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	DXCALL(m_device->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&m_cbv_heap)));

	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	DXCALL(m_device->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&m_cbv_imgui_heap)));

}

void Dx12Context::BuildPSO()
{
	if (!m_bound_shader_program)
		return;

	auto dx_vs = dynamic_cast<Dx12VertexShader*>(m_bound_shader_program->GetVertexShader());
	auto dx_fs = dynamic_cast<Dx12FragmentShader*>(m_bound_shader_program->GetFragmentShader());

	const Dx12VertexLayout* dx_vertex_layout = dynamic_cast<const Dx12VertexLayout*>(m_bound_vertex_array->GetVertexLayout());
	Dx12ShaderProgram* dx_shader_program = dynamic_cast<Dx12ShaderProgram*>(m_bound_shader_program);

	Dx12UploadBuffer* upload_buffer = dx_shader_program->GetUploadBuffer();
	
	D3D12_GPU_VIRTUAL_ADDRESS cb_address = upload_buffer->GetResource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc;
	cbv_desc.BufferLocation = cb_address;
	cbv_desc.SizeInBytes = upload_buffer->GetSize();

	m_device->CreateConstantBufferView(&cbv_desc, m_cbv_heap->GetCPUDescriptorHandleForHeapStart());

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
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.RasterizerState.FrontCounterClockwise = true;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = m_back_buffer_format;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = m_depth_stencil_format;
	DXCALL(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(m_pipeline_state.ReleaseAndGetAddressOf())));
}

Context::API Dx12Context::GetApiType() const
{
	return API::DIRECTX12;
}

ID3D12Device* Dx12Context::GetDevice() const
{
	return m_device.Get();
}

ID3D12GraphicsCommandList* Dx12Context::GetCommandList() const
{
	return m_command_list.Get();
}

void Dx12Context::BindVertexArray(VertexArray* va)
{
	m_bound_vertex_array = va;
	if (m_bound_shader_program && m_bound_ib)
		this->BuildPSO();
}

void Dx12Context::BindShaderProgram(ShaderProgram* shaderProgram)
{
	m_bound_shader_program = shaderProgram;
	if (m_bound_vertex_array && m_bound_ib)
		this->BuildPSO();
}

void Dx12Context::BindIndexBuffer(IndexBuffer* ib)
{
	m_bound_ib = ib;
	if (m_bound_vertex_array && m_bound_shader_program)
		this->BuildPSO();
}

void Dx12Context::BindResourcesToPipeline()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_cbv_heap.Get() };
	m_command_list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	m_command_list->SetGraphicsRootSignature(dynamic_cast<Dx12ShaderProgram*>(m_bound_shader_program)->GetRootSignature());

	dynamic_cast<Dx12VertexArray*>(m_bound_vertex_array)->Bind();
	dynamic_cast<Dx12IndexBuffer*>(m_bound_ib)->Bind();

	m_command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_command_list->SetGraphicsRootDescriptorTable(0, m_cbv_heap->GetGPUDescriptorHandleForHeapStart());
}

void Dx12Context::BindImgui()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_cbv_imgui_heap.Get() };
	m_command_list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

void Dx12Context::Begin()
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	DXCALL(m_direct_cmdlist_alloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	DXCALL(m_command_list->Reset(m_direct_cmdlist_alloc.Get(), m_pipeline_state.Get()));

	// Indicate a state transition on the resource usage.
	m_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->GetCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	m_command_list->RSSetViewports(1, &m_viewport);
	m_command_list->RSSetScissorRects(1, &m_scissor_rect);
	
	// Specify the buffers we are going to render to.
	m_command_list->OMSetRenderTargets(1, &this->GetCurrentBackBufferView(), true, &this->GetDepthStencilView());
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_cbv_heap.Get() };
	m_command_list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

}

void Dx12Context::Present()
{
	// swap the back and front buffers
	DXCALL(m_swapchain->Present(m_is_vsync, 0));
}

void Dx12Context::End()
{
	// Indicate a state transition on the resource usage.
	m_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->GetCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	m_current_back_buffer_index = (m_current_back_buffer_index + 1) % SWAPCHAIN_BUFFER_COUNT;

	// Done recording commands.
	DXCALL(m_command_list->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { m_command_list.Get() };
	m_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	this->FlushCommandQueue();
}

unsigned int Dx12Context::GetNrOfFrameResources() const
{
	return NUM_FRAME_RESOURCES;
}

DXGI_FORMAT Dx12Context::GetBackBufferFormat() const
{
	return m_back_buffer_format;
}

ID3D12DescriptorHeap * Dx12Context::GetRtvDescriptorHeap() const
{
	return m_rtv_heap.Get();
}

ID3D12DescriptorHeap * Dx12Context::GetSrvDescriptorHeap() const
{
	return m_cbv_imgui_heap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Context::GetCurrentBackBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_rtv_heap->GetCPUDescriptorHandleForHeapStart(),
		m_current_back_buffer_index,
		m_rtv_descriptor_size);
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Context::GetDepthStencilView() const
{
	return m_dsv_heap->GetCPUDescriptorHandleForHeapStart();
}

ID3D12Resource* Dx12Context::GetCurrentBackBuffer() const
{
	return m_swapchain_buffers[m_current_back_buffer_index].Get();
}