#pragma once

#include "graphics/BindCopyDestBufferCommand.h"

class GLBindCopyDestBufferCommand : public BindCopyDestBufferCommand
{
public:
	GLBindCopyDestBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~GLBindCopyDestBufferCommand();

	virtual void Execute() override;
};