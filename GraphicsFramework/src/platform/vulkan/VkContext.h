#pragma once

#include "graphics/Context.h"

class Window;

class VkContext : public Context
{
public:
	VkContext(Window* window);
	~VkContext();

	virtual void Initialize();
	virtual void Present();
	virtual API GetApiType() const;
};