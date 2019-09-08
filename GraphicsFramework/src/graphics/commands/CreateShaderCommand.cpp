#include "stdafx.h"

#include "CreateShaderCommand.h"

CreateShaderCommand::CreateShaderCommand(ShaderType type):
	m_type(type)
{

}

CreateShaderCommand::~CreateShaderCommand()
{

}

std::string CreateShaderCommand::ToString() const
{
	return "Create shader";
}