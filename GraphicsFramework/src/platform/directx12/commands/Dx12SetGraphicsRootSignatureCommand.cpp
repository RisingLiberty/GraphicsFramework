#include "stdafx.h"

#include "Dx12SetGraphicsRootSignatureCommand.h"

#include "../Dx12HelperMethods.h"

Dx12SetGraphicsRootSignatureCommand::Dx12SetGraphicsRootSignatureCommand(ID3D12RootSignature* rootSignature) :
    m_root_signature(rootSignature)
{

}

Dx12SetGraphicsRootSignatureCommand::~Dx12SetGraphicsRootSignatureCommand()
{

}

std::string Dx12SetGraphicsRootSignatureCommand::ToString() const
{
    return "Set graphics root signature";
}

void Dx12SetGraphicsRootSignatureCommand::Execute()
{
    GetDx12CommandList()->GetApiCommandList()->SetGraphicsRootSignature(m_root_signature);
}