#pragma once

#include "graphics/commands/Command.h"

class Dx12SetGraphicsRootDescTableCommand : public Command
{
public:
    Dx12SetGraphicsRootDescTableCommand(const D3D12_GPU_DESCRIPTOR_HANDLE& gpu_handle);
    virtual ~Dx12SetGraphicsRootDescTableCommand();

    std::string ToString() const override;
    void Execute() override;

private:
    const D3D12_GPU_DESCRIPTOR_HANDLE m_gpu_handle;
};