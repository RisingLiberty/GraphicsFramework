#pragma once

#include "graphics/commands/BindIndexBufferCommand.h"

class Dx11BufferWrapper;

class Dx11BindIndexBufferCommand : public BindIndexBufferCommand
{
public:
	Dx11BindIndexBufferCommand(const IndexBuffer* buffer);
	virtual ~Dx11BindIndexBufferCommand();

	void Execute() override;
};
