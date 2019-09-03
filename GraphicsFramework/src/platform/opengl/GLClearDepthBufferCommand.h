#pragma once

#include "graphics/ClearDepthBufferCommand.h"

class GLClearDepthBufferCommand : public ClearDepthBufferCommand
{
public:
	GLClearDepthBufferCommand(float depthValue);
	virtual ~GLClearDepthBufferCommand();

	virtual void Execute() override;
};