#pragma once

#include "graphics/commands/BindVertexArrayCommand.h"

class Dx11BindVertexArrayCommand : public BindVertexArrayCommand
{
public:
	Dx11BindVertexArrayCommand(const VertexArray* va);
	virtual ~Dx11BindVertexArrayCommand();

	void Execute() override;
};