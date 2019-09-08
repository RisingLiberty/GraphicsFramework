#pragma once

#include "graphics/commands/BindVertexBufferCommand.h"

class GLBindVertexBufferCommand : public BindVertexBufferCommand
{
public:
	GLBindVertexBufferCommand(const VertexBuffer* vb);
	virtual ~GLBindVertexBufferCommand();

	virtual void Execute() override;
};