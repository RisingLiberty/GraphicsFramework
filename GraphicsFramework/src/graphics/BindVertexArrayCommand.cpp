#include "stdafx.h"

#include "BindVertexArrayCommand.h"

BindVertexArrayCommand::BindVertexArrayCommand(const VertexArray* va) :
	m_vertex_array(va)
{

}

BindVertexArrayCommand::~BindVertexArrayCommand()
{

}