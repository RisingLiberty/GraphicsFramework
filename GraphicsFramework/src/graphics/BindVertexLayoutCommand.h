#pragma once

#include "Command.h"

class VertexLayout;

class BindVertexLayoutCommand : public Command
{
public:
	BindVertexLayoutCommand(const VertexLayout* vertexLayout);
	virtual ~BindVertexLayoutCommand();

	virtual void Execute() override = 0;

protected:
	const VertexLayout* m_vertex_layout;
};