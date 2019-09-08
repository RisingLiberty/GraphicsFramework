#pragma once

#include "graphics/CreateShaderCommand.h"

class GLCreateShaderCommand : public CreateShaderCommand
{
public:
	GLCreateShaderCommand(ShaderType type, unsigned int* id);
	virtual ~GLCreateShaderCommand();

	virtual void Execute() override;

private:
	unsigned int* m_id;
};