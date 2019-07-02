#pragma once

#include "graphics/Context.h"

class Window;

class Dx11Context : public Context
{
public:
	struct Dx11Resources
	{
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> device_context;
		ComPtr<IDXGISwapChain> swapchain;
		ComPtr<ID3D11RenderTargetView> backbuffer;
	};

	Dx11Context(Window* window);
	~Dx11Context();

	void ResizeBuffers(unsigned int width, unsigned int height);

	virtual void Present() override;
	virtual API GetApiType() const override;

	Dx11Resources GetResources() const;
	
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	

protected:
	virtual void InitializeImGui() const override;

private:
	void InitD3D(Window* window);
	void SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* view);

private:

	Dx11Resources m_resources;

	D3D_FEATURE_LEVEL m_feature_level;
	bool m_debug_layer_enabled;
	bool m_is_vsync;

	ComPtr<ID3D11RenderTargetView> m_render_target_view;
	ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;
	ComPtr<ID3D11Texture2D> m_depth_stencil_buffer;
	ComPtr<ID3D11Debug> m_debug_layer;

	D3D11_VIEWPORT m_viewport;
};