#pragma once

#include "graphics/Context.h"

class Window;

class Dx12Context : public Context
{
public:
	struct Dx12Resources
	{
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12Fence> fence;
		ComPtr<ID3D12CommandAllocator> command_allocator;
		ComPtr<ID3D12GraphicsCommandList> command_list;
		ComPtr<IDXGISwapChain> swapchain;
		ComPtr<ID3D12CommandQueue> command_queue;
		ComPtr<ID3D12Resource> depth_stencil_buffer;
		ComPtr<ID3D12DescriptorHeap> render_target_view_heap;
		ComPtr<ID3D12DescriptorHeap> depth_stencil_view_heap;
		ComPtr<IDXGIFactory1> dxgi_factory;
		std::vector<ComPtr<IDXGIAdapter1>> adapters;
		std::vector<ComPtr<IDXGIOutput>> outputs;
	};

	Dx12Context(Window* window);
	virtual ~Dx12Context();

	void FlushCommandQueue();

	virtual void Present() override;
	virtual API GetApiType() const override;

	ID3D12Device* GetDevice() const;
	ID3D12GraphicsCommandList* GetCommandList() const;


private:
	void InitD3D(Window* window);
	void ResizeBuffers(unsigned int width, unsigned int height);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackbufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

private:
	static const unsigned int SWAPCHAIN_BUFFER_COUNT = 2;

	Dx12Resources m_resources;
	D3D_FEATURE_LEVEL m_feature_level;
	bool m_is_vsync;

	ComPtr<ID3D12Debug> m_debug_layer;
	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissor_rect;
	DXGI_FORMAT m_backbuffer_format;
	DXGI_FORMAT m_depth_stencil_format;
	std::array<ComPtr<ID3D12Resource>, SWAPCHAIN_BUFFER_COUNT> m_swapchain_buffers;

	unsigned int m_render_target_view_descriptor_size;
	unsigned int m_depth_stencil_view_descriptor_size;
	unsigned int m_constant_buffer_view_descriptor_size;
	unsigned int m_multi_sample_quality;
	unsigned int m_current_fence;
	unsigned int m_current_backbuffer;
};