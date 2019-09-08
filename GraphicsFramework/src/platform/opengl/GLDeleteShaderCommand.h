#pragma once

#include "graphics/DeleteShaderCommand.h"

class GLDeleteShaderCommand : public DeleteShaderCommand
{
public:
	GLDeleteShaderCommand(unsigned int id);
	virtual ~GLDeleteShaderCommand();

	virtual void Execute() override;

private:
	unsigned int m_id;
};