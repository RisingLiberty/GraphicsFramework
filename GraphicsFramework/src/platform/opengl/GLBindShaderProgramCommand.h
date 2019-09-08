#pragma once

#include "graphics/BindShaderProgramCommand.h"

class GLBindShaderProgramCommand : public BindShaderProgramCommand
{
public:
	GLBindShaderProgramCommand(unsigned int programId);
	virtual ~GLBindShaderProgramCommand();

	virtual void Execute() override;

protected:
	unsigned int m_program_id;
};