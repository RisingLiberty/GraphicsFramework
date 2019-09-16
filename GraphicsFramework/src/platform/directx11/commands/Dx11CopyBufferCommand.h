#pragma once

#include "graphics/commands/CopyBufferCommand.h"

class Dx11CopyBufferCommand : public CopyBufferCommand
{
public:
	Dx11CopyBufferCommand(const unsigned int size, ApiBufferWrapper* dest, const ApiBufferWrapper* src);
	virtual ~Dx11CopyBufferCommand();

	void Execute() override;
};