#pragma once

#include "graphics/Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void ClearAllBuffers() override;
	void ClearColorBuffer() override;
	void ClearDepthStencilBuffer() override;
	void ClearDepthBuffer() override;
	void ClearStencilBuffer() override;
};