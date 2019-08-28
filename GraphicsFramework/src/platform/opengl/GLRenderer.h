#pragma once

#include "graphics/Renderer.h"

class GLRenderer : public Renderer
{
public:
	GLRenderer();
	virtual ~GLRenderer();

	virtual void Draw() override;
	virtual void ClearAllBuffers() override;
	virtual void ClearColorBuffer() override;
	virtual void ClearDepthStencilBuffer() override;
	virtual void ClearDepthBuffer() override;
	virtual void ClearStencilBuffer() override;

	virtual void RenderImgui() override;
};