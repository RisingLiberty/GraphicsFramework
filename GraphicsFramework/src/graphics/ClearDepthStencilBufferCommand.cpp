#include "stdafx.h"

#include "ClearDepthStencilBufferCommand.h"

ClearDepthStencilBufferCommand::ClearDepthStencilBufferCommand(float depthValue, unsigned int stencilValue) :
	m_depth_value(depthValue),
	m_stencil_value(stencilValue)
{

}

ClearDepthStencilBufferCommand::~ClearDepthStencilBufferCommand()
{

}

std::string ClearDepthStencilBufferCommand::ToString() const
{
	return "Clear depth stencil buffer";
}