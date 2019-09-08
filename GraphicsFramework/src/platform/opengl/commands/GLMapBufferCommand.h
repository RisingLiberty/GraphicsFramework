#pragma once

#include "graphics/commands/MapBufferCommand.h"

class GLMapBufferCommand : public MapBufferCommand
{
public:
	GLMapBufferCommand(const ApiBufferWrapper* buffer, void** data, unsigned int size);
	virtual ~GLMapBufferCommand();

	virtual void Execute() override;

private:
	unsigned int m_size;
};