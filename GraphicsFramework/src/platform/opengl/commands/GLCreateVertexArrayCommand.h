#pragma once

#include "graphics/commands/Command.h"

class GLCreateVertexArrayCommand : public Command
{
public:
	GLCreateVertexArrayCommand(unsigned int* id);
	virtual ~GLCreateVertexArrayCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int* m_id;
};