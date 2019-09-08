#pragma once

#include "graphics/Command.h"

class GLValidateProgramCommand : public Command
{
public:
	GLValidateProgramCommand(unsigned int programId);
	virtual ~GLValidateProgramCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_program_id;
};