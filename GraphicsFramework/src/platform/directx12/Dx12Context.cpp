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

#include "Dx12Swapchain.h"

#include "Dx12DepthStencilBuffer.h"
#include "Dx12PipelineState.h"

namespace
{
	const int SWAPCHAIN_BUFFER_COUNT = 2;

	void LogOutputDisplayModes(IDXGIOutput * output, Format format)
	{
		UINT count = 0;
		UINT flags = 0;

		// Call with nullptr to get list count.
		output->GetDisplayModeList(format.ToDirectX(), flags, &count, nullptr);

		std::vector<DXGI_MODE_DESC> modeList(count);
		output->GetDisplayModeList(format.ToDirectX(), flags, &count, &modeList[0]);

		for (auto& x : modeList)
		{
			UINT n = x.RefreshRate.Numerator;
			UINT d = x.RefreshRate.Denominator;
			std::wstring text =
				L"Width = " + std::to_wstring(x.Width) + L" " +
				L"Height = " + std::to_wstring(x.Height) + L" " +
				L"Refresh = " + std::to_wstring((float)n / (float)d);

			std::string string_8bit(text.begin(), text.end());
			spdlog::info(string_8bit);
		}
	}

	void LogAdapterOutputs(IDXGIAdapter* adapter, Format format)
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

			LogOutputDisplayModes(output.Get(), format);

			++i;
		}
	}

	void LogAdapters(IDXGIFactory* factory, Format format)
	{
		UINT i = 0;
		IDXGIAdapter* adapter = nullptr;
		std::vector<ComPtr<IDXGIAdapter>> adapterList;
		while (factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
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
			LogAdapterOutputs(adapterList[i].Get(), format);
	}
}

Dx12Context::Dx12Context(Window* window):
	m_window(window)
{
	spdlog::info("Using DirectX 12");

	this->InitD3D(window);
	m_is_vsync = window->GetPropeties().vsync;
}

Dx12Context::~Dx12Context()
{
	//m_command_list->As<Dx12CommandList>()->Close();
	//m_command_list->As<Dx12CommandList>()->Execute();

	m_command_queue->As<Dx12CommandQueue>()->Execute();
	m_command_queue->As<Dx12CommandQueue>()->Flush();
	this->CleanUp();
};

void Dx12Context::PreInitialize()
{
	m_command_queue = std::make_unique<Dx12CommandQueue>(MAX_NR_OF_FRAMES_IN_FLIGHT);
	m_command_list = m_command_queue->As<Dx12CommandQueue>()->GetApiCommandList();
	m_command_list->As<Dx12CommandList>()->Close();

	m_swapchain = std::make_unique<Dx12Swapchain>(m_window, m_back_buffer_format, SWAPCHAIN_BUFFER_COUNT);

	m_rtv_heap = std::make_unique<Dx12RenderTargetViewDescriptorHeap>(SWAPCHAIN_BUFFER_COUNT);
	m_dsv_heap = std::make_unique<Dx12DepthStencilViewDescriptorHeap>(1);

	DXGI_SWAP_CHAIN_DESC sd = m_swapchain->GetDesc();
	this->OnResize(sd.BufferDesc.Width, sd.BufferDesc.Height);

	// Reset the command list to prep for initialization commands.
	m_command_list->As<Dx12CommandList>()->Open();
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
	m_command_queue->As<Dx12CommandQueue>()->Execute();

	// Wait until initialization is complete.
	m_command_queue->As<Dx12CommandQueue>()->Flush();
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
	LogAdapters(m_dxgi_factory.Get(), m_back_buffer_format);
#endif

}

void Dx12Context::OnResize(unsigned int width, unsigned int height)
{
	assert(m_device);
	assert(m_swapchain);

	m_command_list->As<Dx12CommandList>()->Open();

	// Release the previous resources we will be recreating.
	m_depth_stencil_buffer.reset();
	//m_depth_stencil_buffer.Reset();

	// Resize the swap chain.
	m_swapchain->ResizeBuffers(width, height, m_back_buffer_format,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	m_current_back_buffer_index = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtv_heap->GetCpuView());
	for (UINT i = 0; i < SWAPCHAIN_BUFFER_COUNT; i++)
	{
		//m_swapchain_buffers[i] = m_swapchain->GetBuffer(i);
		m_device->CreateRenderTargetView(m_swapchain->GetBuffer(i), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtv_descriptor_size);
	}

	m_depth_stencil_buffer = std::make_unique<Dx12DepthStencilBuffer>(width, height, m_depth_stencil_format);

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = m_depth_stencil_format.ToDirectX();
	dsvDesc.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(m_depth_stencil_buffer->GetApiBuffer(), &dsvDesc, this->GetDepthStencilView());

	// Execute the resize commands.
	m_command_queue->As<Dx12CommandQueue>()->Execute();

	// Update the viewport transform to cover the client area.
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_scissor_rect = { 0, 0, (LONG)width, (LONG)height};
}


void Dx12Context::BuildPSO()
{
	if (!m_bound_shader_program || m_pipeline_state)
		return;

	Dx12VertexShader* dx_vs = m_bound_shader_program->GetVertexShader()->As<Dx12VertexShader>();
	Dx12FragmentShader* dx_fs = m_bound_shader_program->GetFragmentShader()->As<Dx12FragmentShader>();

	const Dx12VertexLayout* dx_vertex_layout = m_bound_vertex_array->GetVertexLayout()->As<Dx12VertexLayout>();
	const Dx12ShaderProgram* dx_shader_program = m_bound_shader_program->As<Dx12ShaderProgram>();

	m_pipeline_state = std::make_unique<Dx12PipelineState>(
		dx_vs->GetByteCode(), dx_fs->GetByteCode(),
		m_rasterizer_settings, m_bound_index_buffer->GetTopology().ToDirectX12(),
		m_back_buffer_format, m_depth_stencil_format,
		dx_vertex_layout->GetLayoutDesc(), dx_shader_program->GetRootSignature()
		);
}

API Dx12Context::GetApiType() const
{
	return API::DIRECTX12;
}

ID3D12Device* Dx12Context::GetDevice() const
{
	return m_device.Get();
}

Dx12CommandQueue* Dx12Context::GetCommandQueue() const
{
	return m_command_queue->As<Dx12CommandQueue>();
}

Dx12CommandList* Dx12Context::GetCommandList() const
{
	return m_command_list->As<Dx12CommandList>();
}

IDXGIFactory* Dx12Context::GetDxgiFactory() const
{
	return m_dxgi_factory.Get();
}

std::unique_ptr<Dx12CommandList> Dx12Context::CreateDirectCommandList() const
{
	return m_command_queue->As<Dx12CommandQueue>()->CreateDirectCommandList();
}

void Dx12Context::BindResourcesToPipeline()
{
	m_command_list->As<Dx12CommandList>()->SetDescriptorHeaps(m_cbv_heap->GetHeap());
	m_command_list->As<Dx12CommandList>()->SetGraphicsRootSignature(m_bound_shader_program->As<Dx12ShaderProgram>()->GetRootSignature());
	m_command_list->As<Dx12CommandList>()->SetGraphicsRootDescriptorTable(m_cbv_heap->GetGpuView());
}

void Dx12Context::BindImgui()
{
	m_command_list->As<Dx12CommandList>()->SetDescriptorHeaps(m_cbv_imgui_heap->GetHeap());
}

void Dx12Context::Begin()
{
	m_command_list->As<Dx12CommandList>()->ResetAlloc();
	m_command_list->As<Dx12CommandList>()->Open();

	if (m_pipeline_state)
		m_command_list->As<Dx12CommandList>()->SetPipelineState(m_pipeline_state->GetPipelineState());
	m_command_list->As<Dx12CommandList>()->SwitchResourceToRenderTarget(this->GetCurrentBackBuffer());
	m_command_list->As<Dx12CommandList>()->SetViewport(m_viewport);
	m_command_list->As<Dx12CommandList>()->SetScissorRects(m_scissor_rect);
	m_command_list->As<Dx12CommandList>()->SetRenderTargets(this->GetCurrentBackBufferView(), this->GetDepthStencilView());
	m_command_list->As<Dx12CommandList>()->SetDescriptorHeaps(m_cbv_heap->GetHeap());
}

void Dx12Context::Present()
{
	// swap the back and front buffers
	m_swapchain->Present();
}

void Dx12Context::End()
{
	// Indicate a state transition on the resource usage.
	m_command_list->As<Dx12CommandList>()->SwitchResourceToPresent(this->GetCurrentBackBuffer());

	m_current_back_buffer_index = (m_current_back_buffer_index + 1) % SWAPCHAIN_BUFFER_COUNT;

	m_command_queue->As<Dx12CommandQueue>()->Execute();
	m_command_queue->As<Dx12CommandQueue>()->Flush();
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
	D3D12_INDEX_BUFFER_VIEW ib_view = m_bound_index_buffer->As<Dx12IndexBuffer>()->GetBufferView();
	m_command_list->As<Dx12CommandList>()->SetIndexBuffer(ib_view);
	m_command_list->As<Dx12CommandList>()->SetPrimitiveTopology(m_bound_index_buffer->GetTopology().ToDirectX());

	if (m_bound_vertex_array && m_bound_shader_program)
		this->BuildPSO();
}

void Dx12Context::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	//m_command_list->As<Dx12CommandList>()->IASetIndexBuffer(nullptr);
	//m_command_list->As<Dx12CommandList>()->IASetPrimitiveTopology(Topology(ETopology::UNDEFINED).ToDirectX());
}

void Dx12Context::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	const Dx12VertexArray* dx_va = m_bound_vertex_array->As<Dx12VertexArray>();
	D3D12_VERTEX_BUFFER_VIEW vb_view = dx_va->GetVertexBufferView();
	m_command_list->As<Dx12CommandList>()->SetVertexBuffer(vb_view);

	if (m_bound_shader_program && m_bound_index_buffer)
		this->BuildPSO();
}

void Dx12Context::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
	//m_command_list->As<Dx12CommandList>()->IASetVertexBuffers(0, 0, nullptr);
}

void Dx12Context::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	if (m_bound_vertex_array && m_bound_index_buffer)
		this->BuildPSO();

	const Dx12ShaderProgram* dx_shader_program = shaderProgram->As<Dx12ShaderProgram>();
	Dx12UploadBuffer* upload_buffer = dx_shader_program->GetUploadBuffer();

	D3D12_GPU_VIRTUAL_ADDRESS cb_address = upload_buffer->GetResource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc;
	cbv_desc.BufferLocation = cb_address;
	cbv_desc.SizeInBytes = upload_buffer->GetSize();

	m_device->CreateConstantBufferView(&cbv_desc, m_cbv_heap->GetCpuView());

}

void Dx12Context::UnbindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
}

ID3D12Resource* Dx12Context::GetCurrentBackBuffer() const
{
	return m_swapchain->GetBuffer(m_current_back_buffer_index);
}