#pragma once

#include "graphics/Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	virtual ~OpenGLRenderer();

	virtual void Present() override;
	virtual void ClearAllBuffers() override;
	virtual void ClearColorBuffer() override;
	virtual void ClearDepthStencilBuffer() override;
	virtual void ClearDepthBuffer() override;
	virtual void ClearStencilBuffer() override;

	virtual void RenderImgui() override;
};