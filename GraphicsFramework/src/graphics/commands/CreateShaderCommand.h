#pragma once

#include "Command.h"

enum class ShaderType
{
	VERTEX,
	FRAGMENT
};

class CreateShaderCommand : public Command
{
public:
	CreateShaderCommand(ShaderType shaderType);
	virtual ~CreateShaderCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override = 0;

protected:
	ShaderType m_type;
};