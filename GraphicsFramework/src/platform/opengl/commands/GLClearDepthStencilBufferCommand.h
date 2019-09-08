#pragma once

#include "graphics/commands/ClearDepthStencilBufferCommand.h"

class GLClearDepthStencilBufferCommand : public ClearDepthStencilBufferCommand
{
public:
	GLClearDepthStencilBufferCommand(float depthValue, unsigned int stencilValue);
	virtual ~GLClearDepthStencilBufferCommand();

	virtual void Execute() override;
};
