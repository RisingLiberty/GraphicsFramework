#pragma once

#include "graphics/commands/DrawIndexedCommand.h"

class GLDrawIndexedCommand : public DrawIndexedCommand
{
public:
	GLDrawIndexedCommand(unsigned int count, Topology topology, Format format);
	virtual ~GLDrawIndexedCommand();

	void Execute();
};