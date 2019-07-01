#pragma once

#include "graphics/Context.h"

class OpenGLContext : public Context
{
public: 
	OpenGLContext(void* windowHandle);
	virtual ~OpenGLContext();

	void Present() override;
	API GetApiType() const;

protected:
	virtual void InitializeImGui() const final override;

private:
	// Handle to device context
	HDC m_hdc;
};