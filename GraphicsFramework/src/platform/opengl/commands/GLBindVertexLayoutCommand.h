#pragma once

#include "graphics/commands/BindVertexLayoutCommand.h"

class GLBindVertexLayoutCommand : public BindVertexLayoutCommand
{
public:
	GLBindVertexLayoutCommand(const VertexLayout* vertexLayout);
	virtual ~GLBindVertexLayoutCommand();

	virtual void Execute() override;
};