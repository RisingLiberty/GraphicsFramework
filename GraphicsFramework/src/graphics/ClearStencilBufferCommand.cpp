#include "stdafx.h"

#include "ClearStencilBufferCommand.h"

ClearStencilBufferCommand::ClearStencilBufferCommand(unsigned int stencilValue)
{

}

ClearStencilBufferCommand::~ClearStencilBufferCommand()
{

}

std::string ClearStencilBufferCommand::ToString() const
{
	return "Clear stencil buffer";
}