#pragma once

#include "graphics/BindVertexLayoutCommand.h"

class GLBindVertexLayoutCommand : public BindVertexLayoutCommand
{
public:
	GLBindVertexLayoutCommand(const VertexLayout* vertexLayout);
	virtual ~GLBindVertexLayoutCommand();

	virtual void Execute() override;
};