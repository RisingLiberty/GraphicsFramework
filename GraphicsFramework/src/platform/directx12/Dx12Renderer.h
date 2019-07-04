#pragma once

#include "graphics/Renderer.h"

class Dx12Renderer : public Renderer
{
public:
	Dx12Renderer();
	virtual ~Dx12Renderer();

	virtual void Present() override;
	virtual void ClearAllBuffers() override;
	virtual void ClearColorBuffer() override;
	virtual void ClearDepthStencilBuffer() override;
	virtual void ClearDepthBuffer() override;
	virtual void ClearStencilBuffer() override;

};