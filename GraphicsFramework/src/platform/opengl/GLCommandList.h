#pragma once

#include "graphics/RasterizerSettings.h"
#include "graphics/CommandList.h"

class GLCommandList : public CommandList
{
public:
	GLCommandList();
	virtual ~GLCommandList();

};