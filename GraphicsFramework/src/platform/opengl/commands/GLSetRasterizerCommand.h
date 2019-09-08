#pragma once

#include "graphics/commands/SetRasterizerCommand.h"

class GLSetRasterizerCommand : public SetRasterizerCommand
{
public:
	GLSetRasterizerCommand(const RasterizerSettings& settings);
	virtual ~GLSetRasterizerCommand();

	virtual void Execute() override;
};