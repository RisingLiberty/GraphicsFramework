#pragma once

#include "graphics/ClearDepthStencilBufferCommand.h"

class GLClearDepthStencilBufferCommand : public ClearDepthStencilBufferCommand
{
public:
	GLClearDepthStencilBufferCommand(float depthValue, unsigned int stencilValue);
	virtual ~GLClearDepthStencilBufferCommand();

	virtual void Execute() override;
};
