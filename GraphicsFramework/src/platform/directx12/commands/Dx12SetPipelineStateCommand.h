#pragma once

#include "graphics/commands/Command.h"

class Dx12PipelineState;

class Dx12SetPipelineStateCommand : public Command
{
public:
    Dx12SetPipelineStateCommand(Dx12PipelineState* pipelineState);
    virtual ~Dx12SetPipelineStateCommand();

    std::string ToString() const override;
    void Execute() override;

private:
    Dx12PipelineState* m_pipeline_state;
};