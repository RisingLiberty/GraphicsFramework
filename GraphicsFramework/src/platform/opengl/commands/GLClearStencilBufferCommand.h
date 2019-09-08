#pragma once

#include "graphics/commands/ClearStencilBufferCommand.h"

class GLClearStencilBufferCommand : public ClearStencilBufferCommand
{
public:
	GLClearStencilBufferCommand(unsigned int stencilValue);
	virtual ~GLClearStencilBufferCommand();

	virtual void Execute() override;
};