#pragma once

#include "graphics/commands/DrawIndexedCommand.h"

class GLDrawIndexedCommand : public DrawIndexedCommand
{
public:
	GLDrawIndexedCommand(unsigned int count, const Topology topology, const Format format);
	virtual ~GLDrawIndexedCommand();

	void Execute();

private:
	const Topology m_topology;
	const Format m_format;
};