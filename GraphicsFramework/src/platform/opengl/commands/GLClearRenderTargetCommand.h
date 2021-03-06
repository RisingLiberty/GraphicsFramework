#pragma once

#include "graphics/commands/ClearRenderTargetCommand.h"

class GLClearRenderTargetCommand : public ClearRenderTargetCommand
{
public:
	GLClearRenderTargetCommand(const std::array<float, 4>& clearColor);
	virtual ~GLClearRenderTargetCommand();

	virtual void Execute() override;
};