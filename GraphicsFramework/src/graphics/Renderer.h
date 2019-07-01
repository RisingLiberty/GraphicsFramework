#pragma once

class Renderer
{
public: 
	Renderer();
	virtual ~Renderer();

	virtual void ClearAllBuffers() = 0;
	virtual void ClearColorBuffer() = 0;
	virtual void ClearDepthStencilBuffer() = 0;
	virtual void ClearDepthBuffer() = 0;
	virtual void ClearStencilBuffer() = 0;
private:

};