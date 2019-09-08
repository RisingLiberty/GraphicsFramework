#pragma once

#include "graphics/commands/ClearDepthBufferCommand.h"

class GLClearDepthBufferCommand : public ClearDepthBufferCommand
{
public:
	GLClearDepthBufferCommand(float depthValue);
	virtual ~GLClearDepthBufferCommand();

	virtual void Execute() override;
};