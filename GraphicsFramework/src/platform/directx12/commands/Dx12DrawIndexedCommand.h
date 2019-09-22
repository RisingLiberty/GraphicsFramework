#pragma once

#include "graphics/commands/DrawIndexedCommand.h"

class Dx12DrawIndexedCommand : public DrawIndexedCommand
{
public:
    Dx12DrawIndexedCommand(const unsigned int count);
    virtual ~Dx12DrawIndexedCommand();

    void Execute() override;

    
};