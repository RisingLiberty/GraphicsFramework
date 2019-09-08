#pragma once

#include "Command.h"

#include "../BufferHelpers.h"

class DrawIndexedCommand : public Command
{
public:
	DrawIndexedCommand(unsigned int count, Topology topology, Format format);
	virtual ~DrawIndexedCommand();

	void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	unsigned int m_count;
	Topology m_topology;
	Format m_format;
};