#pragma once

#include "graphics/commands/CopyBufferCommand.h"

class GLCopyBufferCommand : public CopyBufferCommand
{
public:
	GLCopyBufferCommand(unsigned int size, const ApiBufferWrapper* destination, const ApiBufferWrapper* source);
	virtual ~GLCopyBufferCommand();

	virtual void Execute() override;
};