#pragma once

#include "graphics/BindIndexBufferCommand.h"

class GLBindIndexBufferCommand : public BindIndexBufferCommand
{
public:
	GLBindIndexBufferCommand(const IndexBuffer* ib);
	virtual ~GLBindIndexBufferCommand();

	virtual void Execute();

private:

};