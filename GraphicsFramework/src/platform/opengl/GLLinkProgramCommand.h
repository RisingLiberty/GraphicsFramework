#pragma once

#include "graphics/Command.h"

class GLLinkProgramCommand : public Command
{
public:
	GLLinkProgramCommand(unsigned int programId);
	virtual ~GLLinkProgramCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_program_id;
};