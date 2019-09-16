#pragma once

#include "graphics/commands/Command.h"

class Dx12SetGraphicsRootSignatureCommand : public Command
{
public:
    Dx12SetGraphicsRootSignatureCommand(ID3D12RootSignature* rootSignature);
    virtual ~Dx12SetGraphicsRootSignatureCommand();

    std::string ToString() const override;
    void Execute() override;

private:
    ID3D12RootSignature* m_root_signature;
};