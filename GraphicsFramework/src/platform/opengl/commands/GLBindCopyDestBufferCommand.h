#pragma once

#include "graphics/commands/BindCopyDestBufferCommand.h"

class GLBindCopyDestBufferCommand : public BindCopyDestBufferCommand
{
public:
	GLBindCopyDestBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~GLBindCopyDestBufferCommand();

	virtual void Execute() override;
};