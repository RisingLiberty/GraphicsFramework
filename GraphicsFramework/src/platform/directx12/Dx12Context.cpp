#include "stdafx.h"

#include "Dx12Context.h"
#include "Dx12HelperMethods.h"
#include "Dx12Renderer.h"

#include "graphics/Window.h"

#include "Dx12VertexLayout.h"
#include "Dx12VertexShader.h"
#include "Dx12FragmentShader.h"

namespace
{
}

Dx12Context::Dx12Context(Window* window):
	m_render_target_view_descriptor_size(0),
	m_depth_stencil_view_descriptor_size(0),
	m_constant_buffer_view_descriptor_size(0),
	m_multi_sample_quality(0),
	m_current_fence(0),
	m_current_backbuffer(0)
{
	spdlog::info("Using DirectX 12");
	
	m_backbuffer_format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	m_is_vsync = window->GetPropeties().vsync;
	this->InitD3D(window);
	
	// Open command list
	m_resources.command_list->Reset(m_resources.command_allocator.Get(), nullptr);
}

Dx12Context::~Dx12Context()
{

}

void Dx12Context::Initialize()
{
	m_renderer = std::make_unique<Dx12Renderer>();
}

void Dx12Context::FlushCommandQueue()
{
	m_current_fence++;

	DXCALL(m_resources.command_queue->Signal(m_resources.fence.Get(), m_current_fence));

	if (m_resources.fence->GetCompletedValue() < m_current_fence)
	{
		HANDLE event_handle = CreateEventExA(nullptr, false, false, EVENT_ALL_ACCESS);
		m_resources.fence->SetEventOnCompletion(m_current_fence, event_handle);
		WaitForSingleObject(event_handle, INFINITE);
		CloseHandle(event_handle);
	}
}

void Dx12Context::Present()
{

}

Context::API Dx12Context::GetApiType() const
{
	return Context::API::DIRECTX12;
}

void Dx12Context::BindInputLayout(Dx12VertexLayout* layout)
{
	m_bound_vertex_layout = layout;
}

void Dx12Context::BindRootSignature(ID3D12RootSignature* rootSignature)
{
	m_bound_root_signature = rootSignature;
}

void Dx12Context::BindVertexShader(Dx12VertexShader* vs)
{
	m_bound_vertex_shader = vs;
}

void Dx12Context::BindFragmentShader(Dx12FragmentShader* fs)
{
	m_bound_fragment_shader = fs;
}

ID3D12Device* Dx12Context::GetDevice() const
{
	return m_resources.device.Get();
}

ID3D12GraphicsCommandList* Dx12Context::GetCommandList() const
{
	return m_resources.command_list.Get();
}

unsigned int Dx12Context::GetNrOfFrameResources() const
{
	return NUM_FRAME_RESOURCES;
}

DXGI_FORMAT Dx12Context::GetBackBufferFormat() const
{
	return m_backbuffer_format;
}

ID3D12DescriptorHeap* Dx12Context::GetRtvDescriptorHeap() const
{
	return m_resources.render_target_view_heap.Get();
}

ID3D12DescriptorHeap* Dx12Context::GetSrvDescriptorHeap() const
{
	return m_resources.srv_descriptor_heap.Get();
}

void Dx12Context::InitD3D(Window* window)
{
	DXCALL(CreateDXGIFactory1(IID_PPV_ARGS(m_resources.dxgi_factory.ReleaseAndGetAddressOf())));

	//enum adapters
	int adapterNr = 0;
	ComPtr<IDXGIAdapter1> pAdapter = nullptr;

	while (m_resources.dxgi_factory->EnumAdapters1(adapterNr++, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		pAdapter->GetDesc1(&desc);

		//std::wcout << L"Adapter: " << desc.Description << L"\n";
		m_resources.adapters.emplace_back(pAdapter);
	}

	for (ComPtr<IDXGIAdapter1> pAdapter : m_resources.adapters)
	{
		//enum outputs
		int outputNr = 0;
		ComPtr<IDXGIOutput> pTempOutput;

		while (pAdapter->EnumOutputs(outputNr++, pTempOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			IDXGIOutput* pOutput;
			DXCALL(pTempOutput->QueryInterface(IID_PPV_ARGS(&pOutput)));

			DXGI_OUTPUT_DESC desc;
			pOutput->GetDesc(&desc);

			//std::wcout << L"Output: " << desc.DeviceName << L"\n";
			m_resources.outputs.emplace_back(pOutput);
		}
	}


#if defined(DEBUG) || defined (_DEBUG)
	ComPtr<ID3D12Debug> debugController;
	D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()));
	debugController->EnableDebugLayer();
#endif

	DXCALL(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(m_resources.device.ReleaseAndGetAddressOf())));

#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> pInfoQueue;
	if (SUCCEEDED(m_resources.device.As(&pInfoQueue)))
	{
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
		// Suppress whole categories of messages
	//D3D12_MESSAGE_CATEGORY Categories[] = {};

	// Suppress messages based on their severity level
		D3D12_MESSAGE_SEVERITY Severities[] =
		{
			D3D12_MESSAGE_SEVERITY_INFO
		};

		// Suppress individual messages by their ID
		D3D12_MESSAGE_ID DenyIds[] = {
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
			D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
			D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
		};

		D3D12_INFO_QUEUE_FILTER NewFilter = {};
		//NewFilter.DenyList.NumCategories = _countof(Categories);
		//NewFilter.DenyList.pCategoryList = Categories;
		NewFilter.DenyList.NumSeverities = _countof(Severities);
		NewFilter.DenyList.pSeverityList = Severities;
		NewFilter.DenyList.NumIDs = _countof(DenyIds);
		NewFilter.DenyList.pIDList = DenyIds;

		DXCALL(pInfoQueue->PushStorageFilter(&NewFilter));
	}
#endif


	DXCALL(m_resources.device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_resources.fence.ReleaseAndGetAddressOf())));

	m_render_target_view_descriptor_size = m_resources.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_depth_stencil_view_descriptor_size = m_resources.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_constant_buffer_view_descriptor_size = m_resources.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS multi_sample_quality_levels;
	multi_sample_quality_levels.Format = m_backbuffer_format;
	multi_sample_quality_levels.SampleCount = 4;
	multi_sample_quality_levels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	multi_sample_quality_levels.NumQualityLevels = 0;

	DXCALL(m_resources.device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &multi_sample_quality_levels, sizeof(multi_sample_quality_levels)));

	m_multi_sample_quality = multi_sample_quality_levels.NumQualityLevels;
	ASSERT(m_multi_sample_quality > 0, "unexpected multi sample quality level!");

	D3D12_COMMAND_QUEUE_DESC queue_desc = {};
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

	DXCALL(m_resources.device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(m_resources.command_queue.ReleaseAndGetAddressOf())));
	DXCALL(m_resources.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_resources.command_allocator.ReleaseAndGetAddressOf())));
	DXCALL(m_resources.device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_resources.command_allocator.Get(), nullptr, IID_PPV_ARGS(m_resources.command_list.ReleaseAndGetAddressOf())));

	m_resources.command_list->Close();

	Window::Properties properties = window->GetPropeties();
	HWND handle = (HWND)window->GetHandle();

	m_resources.swapchain.Reset();
	DXGI_SWAP_CHAIN_DESC swapchain_desc;
	swapchain_desc.BufferDesc.Width = properties.width;
	swapchain_desc.BufferDesc.Height = properties.height;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferDesc.Format = m_backbuffer_format;
	swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapchain_desc.SampleDesc.Count = 1;	//no msaa
	swapchain_desc.SampleDesc.Quality = 0;	//no msaa

	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = SWAPCHAIN_BUFFER_COUNT;
	swapchain_desc.OutputWindow = handle;
	swapchain_desc.Windowed = !properties.is_fullscreen;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGIDevice> dxgi_device = nullptr;
	ComPtr<IDXGIAdapter> dxgi_adapter = nullptr;
	ComPtr<IDXGIFactory> dxgi_factory = nullptr;

	DXCALL(m_resources.dxgi_factory->CreateSwapChain(m_resources.command_queue.Get(), &swapchain_desc, m_resources.swapchain.ReleaseAndGetAddressOf()));

	D3D12_DESCRIPTOR_HEAP_DESC render_target_view_desc;
	render_target_view_desc.NumDescriptors = SWAPCHAIN_BUFFER_COUNT;
	render_target_view_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	render_target_view_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	render_target_view_desc.NodeMask = 0;
	DXCALL(m_resources.device->CreateDescriptorHeap(&render_target_view_desc, IID_PPV_ARGS(m_resources.render_target_view_heap.ReleaseAndGetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC depth_stencil_view_heap_desc;
	depth_stencil_view_heap_desc.NumDescriptors = 1;
	depth_stencil_view_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	depth_stencil_view_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	depth_stencil_view_heap_desc.NodeMask = 0;
	DXCALL(m_resources.device->CreateDescriptorHeap(&depth_stencil_view_heap_desc, IID_PPV_ARGS(m_resources.depth_stencil_view_heap.ReleaseAndGetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC shader_resource_view_desc;
	shader_resource_view_desc.NumDescriptors = 1;
	shader_resource_view_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	shader_resource_view_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	shader_resource_view_desc.NodeMask = 0;
	DXCALL(m_resources.device->CreateDescriptorHeap(&shader_resource_view_desc, IID_PPV_ARGS(m_resources.srv_descriptor_heap.ReleaseAndGetAddressOf())));

	this->ResizeBuffers(properties.width, properties.height);
}

void Dx12Context::ResizeBuffers(unsigned int width, unsigned int height)
{
	this->FlushCommandQueue();
	DXCALL(m_resources.command_list->Reset(m_resources.command_allocator.Get(), nullptr));

	for (int i = 0; i < SWAPCHAIN_BUFFER_COUNT; ++i)
		m_swapchain_buffers[i].Reset();

	m_resources.depth_stencil_buffer.Reset();

	DXCALL(m_resources.swapchain->ResizeBuffers(SWAPCHAIN_BUFFER_COUNT, width, height, m_backbuffer_format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
	m_current_backbuffer = 0;
		
	CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view_heap_handle(m_resources.render_target_view_heap->GetCPUDescriptorHandleForHeapStart());
	for (unsigned int i = 0; i < SWAPCHAIN_BUFFER_COUNT; ++i)
	{
		DXCALL(m_resources.swapchain->GetBuffer(i, IID_PPV_ARGS(m_swapchain_buffers[i].GetAddressOf())));
		m_resources.device->CreateRenderTargetView(m_swapchain_buffers[i].Get(), nullptr, render_target_view_heap_handle);
		render_target_view_heap_handle.Offset(1, m_render_target_view_descriptor_size);
	}

	D3D12_RESOURCE_DESC depth_stencil_desc;
	depth_stencil_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depth_stencil_desc.Alignment = 0;
	depth_stencil_desc.Width = width;
	depth_stencil_desc.Height = height;
	depth_stencil_desc.DepthOrArraySize = 1;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.Format = m_depth_stencil_format;

	depth_stencil_desc.SampleDesc.Count = 1;	//no msaa
	depth_stencil_desc.SampleDesc.Quality = 0;	//no msaa

	depth_stencil_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depth_stencil_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE opt_clear;
	opt_clear.Format = m_depth_stencil_format;
	opt_clear.DepthStencil.Depth = 1.0f;
	opt_clear.DepthStencil.Stencil = 0;
	DXCALL(m_resources.device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &depth_stencil_desc, D3D12_RESOURCE_STATE_COMMON, &opt_clear, IID_PPV_ARGS(m_resources.depth_stencil_buffer.ReleaseAndGetAddressOf())));

	m_resources.device->CreateDepthStencilView(m_resources.depth_stencil_buffer.Get(), nullptr, this->GetDepthStencilView());

	m_resources.command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_resources.depth_stencil_buffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	DXCALL(m_resources.command_list->Close());
	ID3D12CommandList* cmdLists[] = { m_resources.command_list.Get() };
	m_resources.command_queue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = (float)width;
	m_viewport.Height = (float)height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_scissor_rect = { 0,0, (LONG)width, (LONG)height };
}

void Dx12Context::BuildPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC ps_desc;

	ZeroMemory(&ps_desc, sizeof(ps_desc));
	ps_desc.InputLayout = { m_bound_vertex_layout->GetLayout().data(), (UINT)m_bound_vertex_layout->GetLayout().size() };
	ps_desc.pRootSignature = m_bound_root_signature;
	ps_desc.VS = { m_bound_vertex_shader->GetCompiledCode()->GetBufferPointer(), m_bound_vertex_shader->GetCompiledCode()->GetBufferSize() };
	ps_desc.PS = { m_bound_fragment_shader->GetCompiledCode()->GetBufferPointer(), m_bound_fragment_shader->GetCompiledCode()->GetBufferSize() };

	ps_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	ps_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	ps_desc.RasterizerState.FrontCounterClockwise = true;

	ps_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	ps_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	ps_desc.SampleMask = UINT_MAX;
	ps_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	ps_desc.NumRenderTargets = 1;
	ps_desc.RTVFormats[0] = m_backbuffer_format;
	ps_desc.SampleDesc.Count = 1;	// no msaa
	ps_desc.SampleDesc.Quality = 0;	// no msaa
	ps_desc.DSVFormat = m_depth_stencil_format;

	DXCALL(m_resources.device->CreateGraphicsPipelineState(&ps_desc, IID_PPV_ARGS(m_resources.m_pipeline_state.ReleaseAndGetAddressOf())));
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Context::GetCurrentBackbufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_resources.render_target_view_heap->GetCPUDescriptorHandleForHeapStart(), m_current_backbuffer, m_render_target_view_descriptor_size);
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Context::GetDepthStencilView() const
{
	return m_resources.depth_stencil_view_heap->GetCPUDescriptorHandleForHeapStart();
}
