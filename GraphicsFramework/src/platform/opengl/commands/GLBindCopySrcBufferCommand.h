#pragma once

#include "graphics/commands/BindCopySrcBufferCommand.h"

class GLBindCopySrcBufferCommand : public BindCopySrcBufferCommand
{
public:
	GLBindCopySrcBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~GLBindCopySrcBufferCommand();

	virtual void Execute() override;
};