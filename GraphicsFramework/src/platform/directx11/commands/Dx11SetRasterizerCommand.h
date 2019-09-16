#pragma once

#include "graphics/commands/SetRasterizerCommand.h"

class Dx11SetRasterizerCommand : public SetRasterizerCommand
{
public:
	Dx11SetRasterizerCommand(const RasterizerSettings& settings);
	virtual ~Dx11SetRasterizerCommand();

	void Execute() override;

};