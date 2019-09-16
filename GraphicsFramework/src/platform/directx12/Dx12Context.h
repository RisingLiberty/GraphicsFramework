#pragma once

#include "graphics/Context.h"

class Window;
template<typename T>
class UploadBuffer;

class VertexBuffer;
class IndexBuffer;
class Dx12VertexShader;
class Dx12FragmentShader;
class VertexArray;
class VertexLayout;
class VertexShader;
class FragmentShader;
class ShaderProgram;

class Dx12VertexBuffer;
class Dx12IndexBuffer;
class Dx12VertexArray;
class Dx12VertexLayout;

class Dx12DescriptorHeap;
class Dx12CommandQueue;
class Dx12CommandList;
class Dx12Swapchain;

class Dx12DepthStencilBuffer;
class Dx12PipelineState;

class Dx12CommandList;

class Dx12Context : public Context
{
public:
	Dx12Context(Window* window);
	virtual ~Dx12Context();

	virtual void PreInitialize() override;
	virtual void Initialize() override;
	virtual void PostInitialize() override;

	virtual void Present() override;
	virtual API GetApiType() const override;

	ID3D12Device* GetDevice() const;
	Dx12CommandQueue* GetCommandQueue() const;
	Dx12CommandList* GetCommandList() const;
	IDXGIFactory* GetDxgiFactory() const;
	std::unique_ptr<Dx12CommandList> CreateDirectCommandList() const;

	void BindResourcesToPipeline();
	void BindImgui();

	void Begin();
	void End();

	unsigned int GetNrOfFrameResources() const;
	DXGI_FORMAT GetBackBufferFormat() const;
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

protected:
	virtual void BindIndexBufferInternal(const IndexBuffer* indexBuffer);
	virtual void UnbindIndexBufferInternal(const IndexBuffer* indexBuffer);

	virtual void BindVertexArrayInternal(const VertexArray* vertexArray);
	virtual void UnbindVertexArrayInternal(const VertexArray* vertexArray);

	virtual void BindShaderProgramInternal(const ShaderProgram* shaderProgram);
	virtual void UnbindShaderProgramInternal(const ShaderProgram* shaderProgram);

private:
	void InitD3D(Window* window);
	void OnResize(unsigned int width, unsigned int height);

	void BuildPSO();

	ID3D12Resource* GetCurrentBackBuffer() const;

private:
	ComPtr<IDXGIFactory4> m_dxgi_factory;
	ComPtr<ID3D12Device> m_device;

	static const int NUM_FRAME_RESOURCES = 1;
	int m_current_back_buffer_index = 0;

	UINT m_rtv_descriptor_size = 0;
	UINT m_dsv_descriptor_size = 0;
	UINT m_cb_srv_uav_descriptor_size = 0;

	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissor_rect;

	D3D_DRIVER_TYPE m_driver_type = D3D_DRIVER_TYPE_HARDWARE;

	std::unique_ptr<Dx12DescriptorHeap> m_dsv_heap;
	std::unique_ptr<Dx12DescriptorHeap> m_rtv_heap;
	std::unique_ptr<Dx12DescriptorHeap> m_cbv_heap;
	std::unique_ptr<Dx12DescriptorHeap> m_cbv_imgui_heap;

	std::unique_ptr<Dx12DepthStencilBuffer> m_depth_stencil_buffer;
	std::unique_ptr<Dx12PipelineState> m_pipeline_state;

	//std::unique_ptr<Dx12CommandQueue> m_command_queue;
	//Dx12CommandList* m_command_list;

	std::unique_ptr<Dx12Swapchain> m_swapchain;

	Window* m_window;
};