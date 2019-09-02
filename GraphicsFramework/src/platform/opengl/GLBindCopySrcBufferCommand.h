#pragma once

#include "graphics/BindCopySrcBufferCommand.h"

class GLBindCopySrcBufferCommand : public BindCopySrcBufferCommand
{
public:
	GLBindCopySrcBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~GLBindCopySrcBufferCommand();

	virtual void Execute() override;
};