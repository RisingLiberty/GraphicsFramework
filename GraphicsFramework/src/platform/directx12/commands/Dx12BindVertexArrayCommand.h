#pragma once

#include "graphics/commands/BindVertexArrayCommand.h"

class Dx12BindVertexArrayCommand : public BindVertexArrayCommand
{
public:
    Dx12BindVertexArrayCommand(const VertexArray* va);
    virtual ~Dx12BindVertexArrayCommand();

    void Execute() override;
};