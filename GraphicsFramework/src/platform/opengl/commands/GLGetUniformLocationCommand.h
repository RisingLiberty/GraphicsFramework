#pragma once

#include "graphics/commands/Command.h"

class GLGetUniformLocationCommand : public Command
{
public:
	GLGetUniformLocationCommand(unsigned int programId, const std::string& uniformName, int* location);
	virtual ~GLGetUniformLocationCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_program_id;
	std::string m_uniform_name;
	int* m_location;
};