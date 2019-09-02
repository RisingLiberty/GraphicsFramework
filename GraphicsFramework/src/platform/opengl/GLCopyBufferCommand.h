#pragma once

#include "graphics/CopyBufferCommand.h"

class GLCopyBufferCommand : public CopyBufferCommand
{
public:
	GLCopyBufferCommand(unsigned int size, const ApiBufferWrapper* destination, const ApiBufferWrapper* source);
	virtual ~GLCopyBufferCommand();

	virtual void Execute() override;
};