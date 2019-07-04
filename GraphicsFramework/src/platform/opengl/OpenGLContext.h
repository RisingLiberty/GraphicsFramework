#pragma once

#include "graphics/Context.h"

class Window;

class OpenGLContext : public Context
{
public: 
	OpenGLContext(Window* window);
	virtual ~OpenGLContext();

	void Present() override;
	API GetApiType() const;

protected:

private:
	// Handle to device context
	HDC m_hdc;
};