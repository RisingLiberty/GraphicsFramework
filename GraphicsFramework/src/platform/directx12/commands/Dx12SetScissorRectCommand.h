#pragma once

#include "graphics/commands/Command.h"

class Dx12SetScissorRectCommand : public Command
{
public:
    Dx12SetScissorRectCommand(const D3D12_RECT& rect);
    virtual ~Dx12SetScissorRectCommand();

    std::string ToString() const override;
    void Execute() override;
    
private:
    const D3D12_RECT m_rect;
};