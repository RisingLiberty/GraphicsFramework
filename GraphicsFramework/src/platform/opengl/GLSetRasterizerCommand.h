#pragma once

#include "graphics/SetRasterizerCommand.h"

class GLSetRasterizerCommand : public SetRasterizerCommand
{
public:
	GLSetRasterizerCommand(const RasterizerSettings& settings);
	virtual ~GLSetRasterizerCommand();

	virtual void Execute() override;
};