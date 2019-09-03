#pragma once

#include "graphics/ClearStencilBufferCommand.h"

class GLClearStencilBufferCommand : public ClearStencilBufferCommand
{
public:
	GLClearStencilBufferCommand(unsigned int stencilValue);
	virtual ~GLClearStencilBufferCommand();

	virtual void Execute() override;
};