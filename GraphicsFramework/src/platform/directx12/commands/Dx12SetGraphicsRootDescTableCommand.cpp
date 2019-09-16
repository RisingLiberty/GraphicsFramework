#include "stdafx.h"

#include "Dx12SetGraphicsRootDescriptorTableCommand.h"

#include "../Dx12HelperMethods.h"

Dx12SetGraphicsRootDescTableCommand::Dx12SetGraphicsRootDescTableCommand(const D3D12_GPU_DESCRIPTOR_HANDLE& gpu_handle) :
    m_gpu_handle(gpu_handle)
{

}

Dx12SetGraphicsRootDescTableCommand::~Dx12SetGraphicsRootDescTableCommand()
{

}

std::string Dx12SetGraphicsRootDescTableCommand::ToString() const
{
    return "Set graphics root descriptor table";
}

void Dx12SetGraphicsRootDescTableCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->SetGraphicsRootDescriptorTable(0, m_gpu_handle);
}