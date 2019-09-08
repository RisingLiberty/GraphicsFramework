#pragma once

#include "graphics/commands/BindVertexArrayCommand.h"

class GLBindVertexArrayCommand : public BindVertexArrayCommand
{
public:
	GLBindVertexArrayCommand(const VertexArray* va);
	virtual ~GLBindVertexArrayCommand();

	virtual void Execute() override;
};