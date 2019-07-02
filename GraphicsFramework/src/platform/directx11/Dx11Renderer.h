#pragma once

#include "graphics/Renderer.h"

class Dx11Renderer : public Renderer
{
public:
	Dx11Renderer();
	~Dx11Renderer();

	void Present() override;
	void ClearAllBuffers() override;
	void ClearColorBuffer() override;
	void ClearDepthStencilBuffer() override;
	void ClearDepthBuffer() override;
	void ClearStencilBuffer() override;

private:
	DirectX::XMVECTORF32 m_clear_color = DirectX::Colors::Red;
};