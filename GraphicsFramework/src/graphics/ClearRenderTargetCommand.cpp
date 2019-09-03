#include "stdafx.h"

#include "ClearRenderTargetCommand.h"

ClearRenderTargetCommand::ClearRenderTargetCommand(const std::array<float, 4>& clearColor):
	m_clear_color(clearColor)
{

}

ClearRenderTargetCommand::~ClearRenderTargetCommand()
{

}

std::string ClearRenderTargetCommand::ToString() const
{
	return "Clear render target";
}
