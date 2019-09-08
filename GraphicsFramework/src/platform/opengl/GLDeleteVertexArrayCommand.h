#pragma once

#include "graphics/Command.h"

class GLDeleteVertexArrayCommand : public Command
{
public:
	GLDeleteVertexArrayCommand(unsigned int* id);
	virtual ~GLDeleteVertexArrayCommand();

	virtual std::string ToString() const override;
	virtual void Execute() override;

private:
	unsigned int* m_id;
};