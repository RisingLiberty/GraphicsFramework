#include "stdafx.h"

#include "ClearDepthBufferCommand.h"

ClearDepthBufferCommand::ClearDepthBufferCommand(const float depthValue) :
	m_depth_value(depthValue)
{

}

ClearDepthBufferCommand::~ClearDepthBufferCommand()
{

}

std::string ClearDepthBufferCommand::ToString() const
{
	return "Clear depth buffer";
}