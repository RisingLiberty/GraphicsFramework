#include "stdafx.h"

#include "Dx12SetPipelineStateCommand.h"

#include "../Dx12PipelineState.h"
#include "../Dx12HelperMethods.h"

Dx12SetPipelineStateCommand::Dx12SetPipelineStateCommand(Dx12PipelineState* pipelineState) :
    m_pipeline_state(pipelineState)
{

}

Dx12SetPipelineStateCommand::~Dx12SetPipelineStateCommand()
{

}

std::string Dx12SetPipelineStateCommand::ToString() const
{
    return "set pipeline state";
}

void Dx12SetPipelineStateCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->SetPipelineState(m_pipeline_state->GetPipelineState());
}