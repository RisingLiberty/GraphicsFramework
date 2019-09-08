#pragma once

#include "graphics/commands/BindIndexBufferCommand.h"

class GLBindIndexBufferCommand : public BindIndexBufferCommand
{
public:
	GLBindIndexBufferCommand(const IndexBuffer* ib);
	virtual ~GLBindIndexBufferCommand();

	virtual void Execute();

private:

};