#pragma once

#include "graphics/BindVertexArrayCommand.h"

class GLBindVertexArrayCommand : public BindVertexArrayCommand
{
public:
	GLBindVertexArrayCommand(const VertexArray* va);
	virtual ~GLBindVertexArrayCommand();

	virtual void Execute() override;
};