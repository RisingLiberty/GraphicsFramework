#pragma once

#include "graphics/commands/BindIndexBufferCommand.h"

class Dx12BindIndexBufferCommand : public BindIndexBufferCommand
{
public:
    Dx12BindIndexBufferCommand(const IndexBuffer* ib);
    virtual ~Dx12BindIndexBufferCommand();

    void Execute() override;
};