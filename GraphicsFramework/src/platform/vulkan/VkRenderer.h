#pragma once

#include "graphics/Renderer.h"

class VkRenderer : public Renderer
{
public: 
	VkRenderer();
	virtual ~VkRenderer();

	virtual void Draw() override;
	virtual void ClearAllBuffers() override;
	virtual void ClearColorBuffer() override;
	virtual void ClearDepthStencilBuffer() override;
	virtual void ClearDepthBuffer() override;
	virtual void ClearStencilBuffer() override;

	virtual void RenderImgui() override;

};