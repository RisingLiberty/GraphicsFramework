#pragma once

#include "graphics/Command.h"

class GLDetachShaderCommand : public Command
{
public:
	GLDetachShaderCommand(unsigned int programId, unsigned int shaderId);
	virtual ~GLDetachShaderCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_program_id;
	unsigned int m_shader_id;
};