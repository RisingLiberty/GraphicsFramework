#include "stdafx.h"

#include "BindVertexLayoutCommand.h"

BindVertexLayoutCommand::BindVertexLayoutCommand(const VertexLayout* vertexLayout):
	m_vertex_layout(vertexLayout)
{

}

BindVertexLayoutCommand::~BindVertexLayoutCommand()
{

}

std::string BindVertexLayoutCommand::ToString() const
{
	return "Bind vertex layout";
}