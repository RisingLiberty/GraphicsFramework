#pragma once

#include "graphics/commands/Command.h"

class Dx12DescriptorHeap;

class Dx12SetDescriptorHeapsCommand : public Command
{
public:
    Dx12SetDescriptorHeapsCommand(const std::vector<Dx12DescriptorHeap*>& descriptorHeap);
    virtual ~Dx12SetDescriptorHeapsCommand();

    std::string ToString() const override;
    void Execute() override;

private:
    std::vector<Dx12DescriptorHeap*> m_descriptor_heaps;
};