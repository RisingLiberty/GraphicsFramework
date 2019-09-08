#include "stdafx.h"

#include "SetRasterizerCommand.h"

SetRasterizerCommand::SetRasterizerCommand(const RasterizerSettings& settings) :
	m_settings(settings)
{

}

SetRasterizerCommand::~SetRasterizerCommand()
{

}

std::string SetRasterizerCommand::ToString() const
{
	return "Set rasterizer settings";
}