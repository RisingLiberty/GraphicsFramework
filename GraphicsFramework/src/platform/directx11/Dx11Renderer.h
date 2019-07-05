#pragma once

#include "graphics/Renderer.h"

class Dx11Renderer : public Renderer
{
public:
	Dx11Renderer(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView);
	virtual ~Dx11Renderer();

	virtual void Present() override;
	virtual void ClearAllBuffers() override;
	virtual void ClearColorBuffer() override;
	virtual void ClearDepthStencilBuffer() override;
	virtual void ClearDepthBuffer() override;
	virtual void ClearStencilBuffer() override;

private:
	ComPtr<ID3D11RenderTargetView> m_render_target_view;
	ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;
};