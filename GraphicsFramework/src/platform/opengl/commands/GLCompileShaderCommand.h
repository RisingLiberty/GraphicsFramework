#pragma once

#include "graphics/commands/CompileShaderCommand.h"

class GLCompileShaderCommand : public CompileShaderCommand
{
public:
	GLCompileShaderCommand(unsigned int shaderId);
	virtual ~GLCompileShaderCommand();

	virtual void Execute() override;

private:
	unsigned int m_shader_id;
};