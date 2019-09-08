#pragma once

#include "graphics/commands/Command.h"

class GLCreateShaderProgramCommand : public Command
{
public:
	GLCreateShaderProgramCommand(unsigned int* id);
	virtual ~GLCreateShaderProgramCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int* m_id;
};