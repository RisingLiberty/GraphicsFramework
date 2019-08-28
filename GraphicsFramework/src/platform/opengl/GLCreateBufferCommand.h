#pragma once

#include "graphics/CreateBufferCommand.h"

class GLCreateBufferCommand : public CreateBufferCommand
{
public:
	GLCreateBufferCommand(unsigned int* id);
	virtual ~GLCreateBufferCommand();

	virtual void Execute() override;

private:
	unsigned int* m_id;
};