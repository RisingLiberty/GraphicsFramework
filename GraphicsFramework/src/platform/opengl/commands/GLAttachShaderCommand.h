#pragma once

#include "graphics/commands/Command.h"

class GLAttachShaderCommand : public Command
{
public:
	GLAttachShaderCommand(unsigned int programId, unsigned int shaderId);
	virtual ~GLAttachShaderCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_program_id;
	unsigned int m_shader_id;
};