#pragma once

#include "graphics/Command.h"

class GLDeleteShaderProgramCommand : public Command
{
public:
	GLDeleteShaderProgramCommand(unsigned int id);
	virtual ~GLDeleteShaderProgramCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_id;
};