#pragma once

#include "Command.h"

class VertexArray;

class BindVertexArrayCommand : public Command
{
public:
	BindVertexArrayCommand(const VertexArray* va);
	virtual ~BindVertexArrayCommand();

	virtual void Execute() override = 0;

protected:
	const VertexArray* m_vertex_array;
};