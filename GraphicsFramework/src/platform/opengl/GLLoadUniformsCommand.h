#pragma once

#include "graphics/Command.h"

#include "graphics/ShaderUniform.h"


class GLLoadUniformsCommand : public Command
{
public:
	GLLoadUniformsCommand(unsigned int programId, std::vector<std::unique_ptr<ShaderUniform>>& uniforms);
	virtual ~GLLoadUniformsCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int m_program_id;
	std::vector<std::unique_ptr<ShaderUniform>>& m_uniforms;
};