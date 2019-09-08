#pragma once

#include "graphics/Command.h"

class GLSetShaderSourceCommand : public Command
{
public:
	GLSetShaderSourceCommand(unsigned int shaderId, const std::string& code);
	virtual ~GLSetShaderSourceCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_shader_id;
	const std::string m_code;
};