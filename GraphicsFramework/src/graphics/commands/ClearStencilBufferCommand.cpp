#include "stdafx.h"

#include "ClearStencilBufferCommand.h"

ClearStencilBufferCommand::ClearStencilBufferCommand(const unsigned int stencilValue):
	m_stencil_value(stencilValue)
{

}

ClearStencilBufferCommand::~ClearStencilBufferCommand()
{

}

std::string ClearStencilBufferCommand::ToString() const
{
	return "Clear stencil buffer";
}