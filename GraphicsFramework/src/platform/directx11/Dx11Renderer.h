#pragma once

#include "graphics/Renderer.h"

class Dx11Renderer : public Renderer
{
public:
	Dx11Renderer(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView);
	~Dx11Renderer();

	void Present() override;
	void ClearAllBuffers() override;
	void ClearColorBuffer() override;
	void ClearDepthStencilBuffer() override;
	void ClearDepthBuffer() override;
	void ClearStencilBuffer() override;

private:
	ComPtr<ID3D11RenderTargetView> m_render_target_view;
	ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;
};