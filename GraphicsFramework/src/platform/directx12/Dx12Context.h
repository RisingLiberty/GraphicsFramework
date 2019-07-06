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


struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	// Bounding box of the geometry defined by this submesh. 
	// This is used in later chapters of the book.
	DirectX::BoundingBox Bounds;
};

static DirectX::XMFLOAT4X4 Identity4x4()
{
	static DirectX::XMFLOAT4X4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}

struct MeshGeometry
{
	// Give it a name so we can look it up by name.
	std::string Name;

	// System memory copies.  Use Blobs because the vertex/index format can be generic.
	// It is up to the client to cast appropriately.  
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

	// Data about the buffers.
	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;

	// A MeshGeometry may store multiple geometries in one vertex/index buffer.
	// Use this container to define the Submesh geometries so we can draw
	// the Submeshes individually.
	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	// We can free this memory after we finish upload to the GPU.
	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader = nullptr;
	}
};

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 WorldViewProj = Identity4x4();
	//DirectX::XMFLOAT4 color;
};

struct MyObjectConstant
{
	DirectX::XMFLOAT4 color;
};

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
	ID3D12GraphicsCommandList* GetCommandList() const;

	void BindVertexArray(VertexArray* va);
	void BindShaderProgram(ShaderProgram* shaderProgram);
	void BindIndexBuffer(IndexBuffer* ib);

	void BindResourcesToPipeline();

	void Begin();
	void End();

	void BindInputLayout(Dx12VertexLayout* layout);
	void BindRootSignature(ID3D12RootSignature* rootSignature);
	void BindVertexShader(Dx12VertexShader* vs);
	void BindFragmentShader(Dx12FragmentShader* fs);

	unsigned int GetNrOfFrameResources() const;
	DXGI_FORMAT GetBackBufferFormat() const;
	ID3D12DescriptorHeap* GetRtvDescriptorHeap() const;
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

	ID3D12Resource* GetCurrentBackBuffer() const;
	ID3D12CommandAllocator* GetCurrentCommandAllocator() const;
	ID3D12CommandQueue* GetCommandQueue() const;
	ID3D12DescriptorHeap* GetDsvHeap() const;
	IDXGISwapChain* GetSwapchain() const { return m_swapchain.Get(); }
	void IncreaseSwapBufferIndex();
	void BindImgui();

private:
	void InitD3D(Window* window);
	void OnResize(unsigned int width, unsigned int height);

	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

	void BuildDescriptorHeaps();
	void BuildPSO();

	float AspectRatio() const;

private:
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgi_factory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	Microsoft::WRL::ComPtr<ID3D12Device> m_device;

	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
	UINT64 m_current_fence_value = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_command_queue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_direct_cmdlist_alloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_command_list;

	static const int SWAPCHAIN_BUFFER_COUNT = 2;
	int m_current_back_buffer_index = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_swapchain_buffers[SWAPCHAIN_BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depth_stencil_buffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtv_heap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsv_heap;

	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissor_rect;

	UINT m_rtv_descriptor_size = 0;
	UINT m_dsv_descriptor_size = 0;
	UINT m_cb_srv_uav_descriptor_size = 0;

	D3D_DRIVER_TYPE m_driver_type = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT m_back_buffer_format = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	Window* m_window;

	ComPtr<ID3D12DescriptorHeap> m_cbv_heap = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_cbv_imgui_heap = nullptr;

	ComPtr<ID3D12PipelineState> m_pipeline_state = nullptr;

	IndexBuffer* m_bound_ib;
	VertexArray* m_bound_vertex_array;
	ShaderProgram* m_bound_shader_program;
};